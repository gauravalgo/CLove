#pragma once

#include "../3rdparty/lua/lua.h"

int l_event_register(lua_State* state);
int l_event_running();

int l_running;
float swap_At;
