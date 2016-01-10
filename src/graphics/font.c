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
typedef struct {
  unsigned int textureid;
  float sizex;
  float sizey;
  float bearingx;
  float bearingy;
  unsigned int advancex;
  unsigned int advancey;
} character;

static struct {
  character characters[300];
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

static void graphics_Font_newTexture(graphics_Font* font) {
  /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &font->tex);
  glBindTexture(GL_TEXTURE_2D, font->tex);
  graphics_Font_setWrap(font, NULL);
  graphics_Font_setFilter(font, &defaultFilter);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, moduleData.g->bitmap.width, moduleData.g->bitmap.rows, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, moduleData.g->bitmap.buffer);
}

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

  // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(font->face, c, FT_LOAD_RENDER))
        continue;

      // Generate texture
      glGenTextures(1, &font->tex);
      glBindTexture(GL_TEXTURE_2D, font->tex);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_ALPHA,
        font->face->glyph->bitmap.width,
        font->face->glyph->bitmap.rows,
        0,
        GL_ALPHA,
        GL_UNSIGNED_BYTE,
        font->face->glyph->bitmap.buffer
        );
      // Set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // Now store character for later use
      character _character = {
        font->tex,
        font->face->glyph->bitmap.width,
        font->face->glyph->bitmap.rows,
        font->face->glyph->bitmap_left,
        font->face->glyph->bitmap_top,
        font->face->glyph->advance.x,
        font->face->glyph->advance.y
      };
      moduleData.characters[c] = _character;
    }

  return 1;
}

void graphics_Font_setFilter(graphics_Font* font, graphics_Filter const* filter) {
  graphics_Texture_setFilter(font->tex, filter);
}

void graphics_Font_getFilter(graphics_Font *font, graphics_Filter *filter) {
  graphics_Texture_getFilter(font->tex, filter);
}

void graphics_Font_setWrap(graphics_Font *font, graphics_Wrap const* wrap) {
  glBindTexture(GL_TEXTURE_2D, font->tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, graphics_WrapMode_clamp);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, graphics_WrapMode_clamp);
}

void graphics_Font_free(graphics_Font* font) {
  FT_Done_Face(font->face);
  FT_Done_Glyph(font->glyph);
  FT_Done_FreeType(moduleData.ft);
  glDeleteTextures(1,&font->tex);
  free(font);
}

void graphics_Font_render(graphics_Font* font, char const* text, int px, int py, float r, float sx, float sy, float ox, float oy, float kx, float ky) {
  graphics_Shader* shader = graphics_getShader();
  graphics_setDefaultShader();
  uint32_t cp;
  glActiveTexture(GL_TEXTURE0);
  while((cp = utf8_scan(&text))) {
      character ch = moduleData.characters[cp];

      int x = px + ch.bearingx;
      int y = py - ch.bearingy;

      glBindTexture(GL_TEXTURE_2D,ch.textureid);
      m4x4_newTransform2d(&moduleData.tr2d, x, y, r, sx, sy, ox, oy, kx, ky);
      graphics_drawArray(&quad, &moduleData.tr2d,  font->ibo, 4, GL_TRIANGLE_STRIP, GL_UNSIGNED_BYTE,
                         graphics_getColor(), quad.w * ch.sizex , quad.h * ch.sizey);

      px += ch.advancex >> 6;//(moduleData.g->advance.x >> 6);
      py += (ch.advancey >> 6);
    }
  graphics_setShader(shader);
}

int graphics_font_init(void) {
  int error = FT_Init_FreeType(&moduleData.ft);
  if(error)
    return 0;
  return 1;
}
