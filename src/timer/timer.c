/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../graphics/graphics.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#ifdef UNIX
#include "../3rdparty/SDL2/include/SDL.h"
#endif
#ifdef WINDOWS
#include "../3rdparty/glfw/include/GLFW/glfw3.h"
#endif
#endif

#include "timer.h"

static float const FpsUpdateTimeout = 1.0f;

static struct {
  float averageDeltaTime;
  float lastFpsUpdate;
  float currentTime;
  float deltaTime;
  float fps;
  int frames;
} moduleData;

float timer_getTime(void) {
#ifdef EMSCRIPTEN
  return emscripten_get_now() / 1000.0f;
#else
#ifdef UNIX
  return SDL_GetTicks() / 1000.0f;
#endif
#ifdef WINDOWS
    return glfwGetTime();
#endif
#endif
}

void timer_step(void) {
  ++moduleData.frames;

  float last = moduleData.currentTime;
  moduleData.currentTime = timer_getTime();
  moduleData.deltaTime = moduleData.currentTime - last;

  float timeSinceLastUpdate = moduleData.currentTime - moduleData.lastFpsUpdate;
  if(timeSinceLastUpdate > FpsUpdateTimeout) {
    moduleData.averageDeltaTime = timeSinceLastUpdate / moduleData.frames;
    moduleData.fps = moduleData.frames / timeSinceLastUpdate;
    moduleData.lastFpsUpdate = moduleData.currentTime;
    moduleData.frames = 0;
  }
}


float timer_getFPS(void) {
  return moduleData.fps;
}

float timer_getDelta(void) {
  return moduleData.deltaTime;
}

float timer_getAverageDelta(void) {
  return moduleData.averageDeltaTime;
}

void timer_init() {
  float now = timer_getTime();
  moduleData.averageDeltaTime = 0.0;
  moduleData.lastFpsUpdate = now;
  moduleData.currentTime = now;
  moduleData.deltaTime = 0.0f;
  moduleData.fps = 0.0f;
  moduleData.frames = 0;
}
