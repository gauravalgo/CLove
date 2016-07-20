/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../3rdparty/stb/stb_image.c"
#include "imagedata.h"


int image_ImageData_new_with_filename(image_ImageData *dst, char const* filename) {
  int n;
  dst->surface = stbi_load(filename, &dst->w, &dst->h, &n, STBI_rgb_alpha);
  dst->c = STBI_rgb_alpha;
  
  if(!dst->surface) //image could not be loaded
    return 0;

  return 1;
}

void image_ImageData_new_with_size(image_ImageData *dst, int width, int height) {
  dst->surface = malloc(sizeof(unsigned char) * width * height * 4);
  dst->w = width;
  dst->h = height;
  memset(dst->surface, 0, sizeof(unsigned char) * width * height * 4);
}

// return rgba(4) or rgb(3) depending on the image type
int image_ImageData_getChannels(image_ImageData *dst) {
	return dst->c;
}

int image_ImageData_getPixel(image_ImageData *dst, int x, int y) {
	return dst->surface[y * dst->w + x];
}

int image_ImageData_setPixel(image_ImageData *dst, int x, int y, pixel p) {
	pixel *pixels = (pixel*) dst->surface;
	pixels[y * dst->w + x] = p;
	
	return 1;
}

unsigned char* image_ImageData_getSurface(image_ImageData *dst) {
	return dst->surface;
}

int image_ImageData_getWidth(image_ImageData *dst) {
	return dst->w;
}

int image_ImageData_getHeight(image_ImageData *dst) {
	return dst->h;
}

const char* image_error(void){
  return stbi_failure_reason();
}

void image_ImageData_free(image_ImageData *data) {
  free(data->surface);
}

void image_init() {
}
