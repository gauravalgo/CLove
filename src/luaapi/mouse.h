/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../3rdparty/lua/lua.h"

#include "../graphics/graphics.h"

#ifdef UNIX
#include "../3rdparty/SDL2/include/SDL.h"
#endif
#ifdef WINDOWS
#include "../3rdparty/glfw/include/GLFW/glfw3.h"
#endif

void l_mouse_register(lua_State* state);
void l_mouse_pressed(int x, int y, int button);
void l_mouse_released(int x, int y, int button);
void l_mouse_wheelmoved(int y);
