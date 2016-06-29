/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../3rdparty/openal/include/AL/al.h"
#include "../3rdparty/libmpg123/config/mac/x64/mpg123.h"

#include <stdio.h>

typedef struct {
  size_t buffer_size;
  int channels, encoding;
  long rate;
} audio_mp3_DecoderData;


int audio_mp3_load(unsigned int bufferID, const char* filename);
