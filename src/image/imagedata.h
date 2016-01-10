/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include <stdint.h>

typedef struct {
  int w;
  int h;
  uint8_t *surface;
} image_ImageData;

char const* image_error(void);
void image_ImageData_new_with_size(image_ImageData *dst, int width, int height);
int image_ImageData_new_with_filename(image_ImageData *dst, char const* filename);
void image_ImageData_free(image_ImageData *data);
void image_init();
