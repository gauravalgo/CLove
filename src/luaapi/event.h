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

int l_event_register(lua_State* state);
int l_event_running();

//TODO rename these using event_ as the prefix
int l_running;
bool event_force_quit;
float swap_At;
