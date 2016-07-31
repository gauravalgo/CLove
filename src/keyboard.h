/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "graphics/graphics.h"

#ifdef UNIX
#include "3rdparty/SDL2/include/SDL_keyboard.h"
#endif

#ifdef WINDOWS

#include "3rdparty/glfw/include/GLFW/glfw3.h"

#define MAX_KEYS GLFW_KEY_LAST
void keyboard_setcallback(void);

// Don't know if this is the best approach but I don't care at this point
bool DownKey[MAX_KEYS];
bool UpKey[MAX_KEYS];
int keyboardButton; // the current key that has been pressed

#endif

void keyboard_init(void);

#ifdef UNIX
char const * keyboard_getKeyName(SDL_Keycode key);
void keyboard_keypressed(SDL_Keycode key);
void keyboard_keyreleased(SDL_Keycode key);
bool keyboard_ispressed(SDL_Keycode key);
SDL_Keycode keyboard_getKeycode(char const* name);
#endif
#ifdef WINDOWS
char const * keyboard_getKeyName(int key);
void keyboard_keypressed(int key);
void keyboard_keyreleased(int key);
bool keyboard_ispressed(int key);
#endif

void keyboard_startText(void);
void keyboard_stopText(void);
bool keyboard_isTextEnabled(void);
void keyboard_textInput(char const* text);
