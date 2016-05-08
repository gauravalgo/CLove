/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include "../math/util.h"
#include <math.h>
#include "geometry.h"
#include "graphics.h"
#include "shader.h"
#include "matrixstack.h"
#include "vertex.h"

static struct {
  graphics_Shader plainColorShader;
  mat4x4 tr2d;
  GLuint vbo;
  GLuint ibo;
  GLuint vao;
} moduleData;

static graphics_Vertex const point_vertices[] = {
  {{1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
};


static graphics_Vertex const square_vertices[] = {
  {{1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
  {{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
};

static unsigned char const square_indices[] = { 0, 1, 2, 3};

void graphics_geometry_init(void) {
  
#ifdef __MACH__
  glGenVertexArrays(1, &moduleData.vao);
  glBindVertexArray(moduleData.vao);
#endif
  glGenBuffers(1, &moduleData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);


  glGenBuffers(1, &moduleData.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_indices), square_indices, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(2*sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(4*sizeof(float)));

  graphics_Shader_new(&moduleData.plainColorShader, NULL,
                      "vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_cords ) {\n"
                      "  return color;\n"
                      "}\n");

}

void graphics_geometry_free () {
  glDeleteBuffers(1, &moduleData.ibo);
  glDeleteBuffers(1, &moduleData.vbo);
#ifdef __MACH__
  glDeleteBuffers(1, &moduleData.vao);
#endif
  graphics_Shader_free(&moduleData.plainColorShader);
}

static void growBuffers(int vertices, int indices) {

}

static graphics_Quad quad = {0.0f,0.0f,1.0f,1.0f};
static void drawBuffer(int vertices, int indices, GLenum type) {

}

void graphics_geometry_drawCircle(float x, float y, float radius, int segments) {
  growBuffers(segments*2, segments*2+2);


  drawBuffer(segments*2, segments*2+2, GL_TRIANGLE_STRIP);
}


void graphics_geometry_fillCircle(float x, float y, float radius, int segments) {
  growBuffers(segments+1, segments+2);


  drawBuffer(segments+1, segments+2, GL_TRIANGLE_FAN);
}

void graphics_geometry_fillRectangle(float x, float y, float w, float h) {
  graphics_Shader *shader = graphics_getShader();

  graphics_Image img = {
    0, w, h
  };
  graphics_setShader(&moduleData.plainColorShader);
  graphics_Image_draw(&img, &quad, x, y, 0, 1, 1, 0, 0, 0, 0);

  graphics_setShader(shader);
}

void graphics_geometry_drawRectangle(float x, float y, float w, float h) {
  graphics_Shader *shader = graphics_getShader();
  graphics_setShader(&moduleData.plainColorShader);

  m4x4_newTransform2d(&moduleData.tr2d, x, y, 0, 1, 1, 0, 0, 0, 0);
  glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_DYNAMIC_DRAW);
#ifdef __MACH__
  graphics_drawArrayVAO(&quad, &moduleData.tr2d,  moduleData.ibo, moduleData.vao, 4, GL_LINE_LOOP, GL_UNSIGNED_BYTE,
                     graphics_getColor(), w * quad.w, h * quad.h);
#else 
    graphics_drawArray(&quad, &moduleData.tr2d,  moduleData.ibo, 4, GL_LINE_LOOP, GL_UNSIGNED_BYTE,
                     graphics_getColor(), w * quad.w, h * quad.h);
#endif
  graphics_setShader(shader);
}

void graphics_geometry_points(float x, float y) {
  graphics_Shader *shader = graphics_getShader();
  graphics_setShader(&moduleData.plainColorShader);
  m4x4_newTransform2d(&moduleData.tr2d, x, y, 0, 1, 1, 0, 0, 0, 0);
  glBufferData(GL_ARRAY_BUFFER, sizeof(point_vertices), point_vertices, GL_DYNAMIC_DRAW);
#ifdef __MACH__
  graphics_drawArrayVAO(&quad, &moduleData.tr2d,  moduleData.ibo, moduleData.vao, 4, GL_POINTS, GL_UNSIGNED_BYTE,
                     graphics_getColor(), quad.w, quad.h);
#else 
  graphics_drawArray(&quad, &moduleData.tr2d,  moduleData.ibo, 4, GL_POINTS, GL_UNSIGNED_BYTE,
                     graphics_getColor(), quad.w, quad.h);
#endif
  graphics_setShader(shader);
}












