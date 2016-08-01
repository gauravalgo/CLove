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
#include "../3rdparty/lua/lua.h"

#include "../graphics/graphics.h"

#ifdef UNIX
#include "../3rdparty/SDL2/include/SDL_keycode.h"
#endif

#ifdef WINDOWS
#include "../3rdparty/glfw/include/GLFW/glfw3.h"
#endif

void l_keyboard_register(lua_State* state);

#ifdef UNIX
void l_keyboard_keypressed(SDL_Keycode key, bool repeat);
void l_keyboard_keyreleased(SDL_Keycode key);
#endif

#ifdef WINDOWS
void l_keyboard_keypressed(const char *key, bool repeat);
void l_keyboard_keyreleased(const char *key);
#endif

void l_keyboard_textInput(const char* text);
