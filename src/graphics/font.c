/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#define _POSIX_SOURCE
#include <string.h>
#include "font.h"
#include <stdlib.h>
#include <stdio.h>
#include "quad.h"
#include "graphics.h"
#include "../tools/utf8.h"
#include "../math/minmax.h"
#include "shader.h"
#include "batch.h"
#include "vera_ttf.c"
#include "../3rdparty/freetype/include/ft2build.h"
#include "../3rdparty/freetype/include/freetype.h"
#include "../3rdparty/freetype/include/ftglyph.h"

#include FT_GLYPH_H

static struct {
  FT_Library ft;
  mat4x4 tr2d;
  FT_GlyphSlot g;
} moduleData;

static graphics_Vertex const imageVertices[] = {
  {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
};

static graphics_Quad const quad = {
  0.0f,0.0f,1.0f,1.0f
};

static const graphics_Filter defaultFilter = {
  .maxAnisotropy = 1.0f,
  .mipmapLodBias = 1.0f,
  .minMode = graphics_FilterMode_linear,
  .magMode = graphics_FilterMode_linear,
  .mipmapMode = graphics_FilterMode_none
};

int graphics_Font_new(graphics_Font* font, char const* filename, int ptsize) {
  if(filename){
      if (FT_New_Face(moduleData.ft, filename, 0, &font->face)) {
          fprintf(stderr, "Could not open font %s\n", filename);
          return 0;
        }
    }else
    FT_New_Memory_Face(moduleData.ft, defaultFontData, defaultFontSize, 0, &font->face);

  FT_Set_Pixel_Sizes(font->face, 0, ptsize);

  glGenBuffers(1, &font->vbo);
  glGenBuffers(1, &font->ibo);


  glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STATIC_DRAW);
  unsigned char const imageIndices[] = { 0, 1, 2, 3 };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, font->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(imageIndices), imageIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(2*sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(4*sizeof(float)));

  moduleData.g = font->face->glyph;

  glGenTextures(1, &font->tex);
  glBindTexture(GL_TEXTURE_2D, font->tex);
  graphics_Font_setWrap(font, NULL);
  graphics_Font_setFilter(font, &defaultFilter);
  return 1;
}

void graphics_Font_setFilter(graphics_Font *img, graphics_Filter const* filter) {
  graphics_Texture_setFilter(img->tex, filter);
}

void graphics_Font_getFilter(graphics_Font *img, graphics_Filter *filter) {
  graphics_Texture_getFilter(img->tex, filter);
}

void graphics_Font_setWrap(graphics_Font *img, graphics_Wrap const* wrap) {
  glBindTexture(GL_TEXTURE_2D, img->tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, graphics_WrapMode_clamp);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, graphics_WrapMode_clamp);
}

void graphics_Font_free(graphics_Font* font) {
  FT_Done_Glyph(font->glyph);
  free(font);
}

void graphics_Font_render(graphics_Font* font, char const* text, int px, int py, float r, float sx, float sy, float ox, float oy, float kx, float ky) {
  //  graphics_Shader* shader = graphics_getShader();
  // graphics_setDefaultShader();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font->tex);

  /* Loop through all characters */
  for (font->text = text; *font->text; font->text++) {
      /* Try to load and render the character */
      if (FT_Load_Char(font->face, *font->text, FT_LOAD_RENDER))
        continue;

      int x = px + moduleData.g->bitmap_left;
      int y = py - moduleData.g->bitmap_top;

      /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
      glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, moduleData.g->bitmap.width, moduleData.g->bitmap.rows, 0, GL_ALPHA,
                   GL_UNSIGNED_BYTE, moduleData.g->bitmap.buffer);

      m4x4_newTransform2d(&moduleData.tr2d, x, y, r, sx, sy, ox, oy, kx, ky);
      graphics_drawArray(&quad, &moduleData.tr2d,  font->ibo, 4, GL_TRIANGLE_STRIP, GL_UNSIGNED_BYTE,
                         graphics_getColor(), quad.w * moduleData.g->bitmap.width, quad.h * moduleData.g->bitmap.rows);

      px += (moduleData.g->advance.x >> 6);
      py += (moduleData.g->advance.y >> 6);
    }
  //graphics_setShader(shader);
}

int graphics_font_init(void) {
  int error = FT_Init_FreeType(&moduleData.ft);
  if(error)
    return 0;
  return 1;
}
