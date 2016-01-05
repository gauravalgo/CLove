#include <stdlib.h>
#include "../math/util.h"
#include <math.h>
#include "geometry.h"
#include "graphics.h"
#include "shader.h"
#include "matrixstack.h"

static struct {
  unsigned int dataVBO;
  unsigned int dataIBO;
  int currentDataSize;
  int currentIndexSize;
  float *data;
  unsigned short *index;
  graphics_Shader plainColorShader;
  float lineWidth;
  float pointSize;
  mat4x4 tr;
} moduleData;


void graphics_geometry_init(void) {
  glGenBuffers(1, &moduleData.dataIBO);
  glGenBuffers(1, &moduleData.dataVBO);
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.dataVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.dataIBO);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));

  graphics_Shader_new(&moduleData.plainColorShader, NULL,
    "vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_cords ) {\n"
    "  return color;\n"
    "}\n");

  moduleData.lineWidth = 1.0f;
  m4x4_newIdentity(&moduleData.tr);
}


static void growBuffers(int vertices, int indices) {
  int datasize = vertices * 6 * sizeof(float);
  if(moduleData.currentDataSize < datasize) {
    free(moduleData.data);
    moduleData.data = (float*)malloc(datasize);
    moduleData.currentDataSize = datasize;
  }

  int indexsize = indices * sizeof(unsigned short);
  if(moduleData.currentIndexSize < indexsize) {
    free(moduleData.index);
    moduleData.index = (unsigned short*)malloc(indexsize);
    moduleData.currentIndexSize = indexsize;
  }
}

static graphics_Quad quad = {0.0f,0.0f,1.0f,1.0f};
static void drawBuffer(int vertices, int indices, GLenum type) {
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.dataVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices*6*sizeof(float), moduleData.data, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.dataIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices*sizeof(uint16_t), moduleData.index, GL_DYNAMIC_DRAW);

  graphics_Shader *shader = graphics_getShader();
  graphics_setShader(&moduleData.plainColorShader);

  graphics_drawArray(&quad, &moduleData.tr, moduleData.dataIBO, indices, type, GL_UNSIGNED_SHORT, graphics_getColor(),1,1);

  graphics_setShader(shader);
}

void graphics_geometry_drawCircle(float x, float y, float radius, int segments) {
  growBuffers(segments*2, segments*2+2);

  float step = 2*PI / segments;
  float ang = 0;
  moduleData.index[2*segments  ] = 0;
  moduleData.index[2*segments+1] = 1;
  float lwh = moduleData.lineWidth / 2.0f;
  for(int i = 0; i < segments; ++i, ang += step) {
    float s = sin(ang);
    float c = cos(ang);
    float * base = moduleData.data + 12*i;

    // outer vertex
    base[0] = s * (radius+lwh) + x;
    base[1] = c * (radius+lwh) + y;
    base[2]  = 1.0f;
    base[3]  = 1.0f;
    base[4]  = 1.0f;
    base[5]  = 1.0f;

    // inner vertex
    base[6] = s * (radius-lwh) + x;
    base[7] = c * (radius-lwh) + y;
    base[8]  = 1.0f;
    base[9]  = 1.0f;
    base[10] = 1.0f;
    base[11] = 1.0f;

    moduleData.index[2*i] = 2*i;
    moduleData.index[2*i+1] = 2*i+1;
  }

  drawBuffer(segments*2, segments*2+2, GL_TRIANGLE_STRIP);
}


void graphics_geometry_fillCircle(float x, float y, float radius, int segments) {
  growBuffers(segments+1, segments+2);

  float step = 2*PI / segments;
  float ang = 0;
  moduleData.data[0] = x;
  moduleData.data[1] = y;
  moduleData.data[2] = 1.0f;
  moduleData.data[3] = 1.0f;
  moduleData.data[4] = 1.0f;
  moduleData.data[5] = 1.0f;
  moduleData.index[0] = 0;
  moduleData.index[segments+1] = 1;
  for(int i = 0; i < segments; ++i, ang -= step) {
    float * base = moduleData.data + 6*(i+1);
    base[0] = sin(ang) * radius + x;
    base[1] = cos(ang) * radius + y;
    base[2] = 1.0f;
    base[3] = 1.0f;
    base[4] = 1.0f;
    base[5] = 1.0f;
    moduleData.index[i+1] = i+1;
  }

  drawBuffer(segments+1, segments+2, GL_TRIANGLE_FAN);
}

void graphics_geometry_fillRectangle(float x, float y, float w, float h) {
  graphics_Shader *shader = graphics_getShader();

  graphics_Image img = {
    0, w, h
  };
  graphics_Quad q = {0,0,1,1};
  graphics_setShader(&moduleData.plainColorShader);
  graphics_Image_draw(&img, &q, x, y, 0, 1, 1, 0, 0, 0, 0);

  graphics_setShader(shader);
}


void graphics_geometry_drawRectangle(float x, float y, float w, float h) {
  growBuffers(8, 10);

  float *buf = moduleData.data;
  float lwh = moduleData.lineWidth / 2.0f;

  // top left
  buf[0] = x - lwh;
  buf[1] = y - lwh;

  buf[6] = x + lwh;
  buf[7] = y + lwh;

  float x2 = x + w;
  float y2 = y + h;

  // top right
  buf[12] = x2 + lwh;
  buf[13] = y  - lwh;

  buf[18] = x2 - lwh;
  buf[19] = y  + lwh;

  // bottom right
  buf[24] = x2 + lwh;
  buf[25] = y2 + lwh;

  buf[30] = x2 - lwh;
  buf[31] = y2 - lwh;

  // bottom left
  buf[36] = x - lwh;
  buf[37] = y2 + lwh;

  buf[42] = x + lwh;
  buf[43] = y2 - lwh;

  for(int i = 0; i < 8; ++i) {
    for(int j = 2; j < 6; ++j) {
      buf[6*i+j] = 1.0f;
    }
    moduleData.index[i] = i;
  }
  moduleData.index[8] = 0;
  moduleData.index[9] = 1;

  drawBuffer(8, 10, GL_TRIANGLE_STRIP);
}

float graphics_geometry_getLineWidth(void) {
  return moduleData.lineWidth;
}


void graphics_geometry_setLineWidth(float width) {
  moduleData.lineWidth = width;
}

void graphics_geometry_setPointSize(float size) {
  moduleData.pointSize = size;
}

float graphics_geometry_getPointSize() {
  return moduleData.pointSize;
}
