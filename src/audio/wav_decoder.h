#pragma once

#include "../3rdparty/openal/include/AL/al.h"
#include <stdio.h>

typedef struct {
  ALshort    *readBuffer;
  int         readBufferSize;
  int         preloadedSamples;
  int         size;
  int         samplerate;
} audio_wav_DecoderData;


int audio_wav_load(unsigned int buffer,char const* filename);
