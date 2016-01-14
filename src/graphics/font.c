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
  const char* name;
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

static const graphics_Wrap defaultWrap = {
  .verMode = graphics_WrapMode_clamp,
  .horMode = graphics_WrapMode_clamp
};

int graphics_font_init(void) {
  int error = FT_Init_FreeType(&moduleData.ft);
  if(error){
      printf("%s \n","Could not init freetype");
      return 0;
    }
  return 1;
}

void graphics_Font_free(graphics_Font* font) {
  FT_Done_Face(font->face);
  FT_Done_FreeType(moduleData.ft);
  glDeleteTextures(1,&font->tex);
}

static void graphics_Font_newTexture(graphics_Font* font) {
  /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &font->tex);
  glBindTexture(GL_TEXTURE_2D, font->tex);

  FT_Bitmap b = moduleData.g->bitmap;

    // Create LUMINANCE_ALPHA texture data
    // TODO: Maybe just alpha (or GL_R on modern core profiles) is
    //       enough?
    uint8_t *buf = malloc(2*b.rows*b.width + 10);
    uint8_t *row = b.buffer;
    for(int i = 0; i < b.rows; ++i) {
      for(int c = 0; c < b.width; ++c) {
        buf[2*(i*b.width + c)    ] = 255;
        buf[2*(i*b.width + c) + 1] = row[c];
      }
      row += b.pitch;
    }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               moduleData.g->bitmap.width, moduleData.g->bitmap.rows, 0, GL_LUMINANCE_ALPHA,
               GL_UNSIGNED_BYTE, buf);

  graphics_Font_setWrap(font, &defaultWrap);
  graphics_Font_setFilter(font, &defaultFilter);
}

void graphics_Font_setName(graphics_Font* font, const char *name) {
  font->name = name;
}

const char* graphics_Font_getName(graphics_Font* font) {
  return font->name;
}

int graphics_Font_new(graphics_Font* font, char const* filename, int ptsize) {
  if(filename){
      if (FT_New_Face(moduleData.ft, filename, 0, &font->face) != 0) {
          fprintf(stderr, "Could not open font %s \n", filename);
          return 1;
        }
      graphics_Font_setName(font, filename);
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
  for (GLubyte c = 0; c < 128; c++){
      // Load character glyph
      if (FT_Load_Char(font->face, c, FT_LOAD_RENDER))
        continue;

      graphics_Font_newTexture(font);
      // Now store character for later use
      character _character = {
        font->tex,
        moduleData.g->bitmap.width,
        moduleData.g->bitmap.rows,
        moduleData.g->bitmap_left,
        moduleData.g->bitmap_top,
        moduleData.g->advance.x,
        moduleData.g->advance.y
      };
      moduleData.characters[c] = _character;
    }

  return 0;
}

void graphics_Font_render(graphics_Font* font, char const* text, int px, int py, float r, float sx, float sy, float ox, float oy, float kx, float ky) {
  uint32_t cp;
  character ch;
  int x = 0;
  int storeX = 0;
  int y = font->face->ascender;
  graphics_Shader* shader = graphics_getShader();
  graphics_setDefaultShader();

  while((cp = utf8_scan(&text))) {
      ch = moduleData.characters[cp];
      glBindTexture(GL_TEXTURE_2D,ch.textureid);

      if (storeX == 0)
        storeX = px - ch.sizex;

      x = px + ch.bearingx;
      y = py - ch.bearingy;

      if (cp == '\n'){
          px = x - (((ch.advancex >> 5)) * (ch.sizex + ch.bearingx));
          if (px < storeX)
            px = storeX;
          py += floor(ch.bearingy + 5.25f);
          continue;
        }

      m4x4_newTransform2d(&moduleData.tr2d, x, y, r, sx, sy, ox, oy, kx, ky);
      graphics_drawArray(&quad, &moduleData.tr2d,  font->ibo, 4, GL_TRIANGLE_STRIP, GL_UNSIGNED_BYTE,
                         graphics_getColor(), quad.w * ch.sizex , quad.h * ch.sizey);

      px += ch.advancex >> 6;
      py += ch.advancey >> 6;
    }
  graphics_setShader(shader);
}

void graphics_Font_setFilter(graphics_Font* font, graphics_Filter const* filter) {
  graphics_Texture_setFilter(font->tex, filter);
}

void graphics_Font_getFilter(graphics_Font *font, graphics_Filter *filter) {
  graphics_Texture_getFilter(font->tex, filter);
}

void graphics_Font_setWrap(graphics_Font *font, graphics_Wrap const* wrap) {
  glBindTexture(GL_TEXTURE_2D, font->tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap->horMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap->verMode);
}

int graphics_Font_getDescent(graphics_Font* font) {
  return font->face->descender;
}

int graphics_Font_getBaseline(graphics_Font* font) {
  return floor(font->glyph->bitmap.rows / 1.25f + .5f);
}

int graphics_Font_getAscent(graphics_Font* font) {
  return font->face->ascender;
}

int graphics_Font_getHeight(graphics_Font* font){
  return font->glyph->bitmap.rows;
}

