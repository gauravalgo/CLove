/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "event.h"
#include "tools.h"

static int l_event_quit(lua_State* state){
  l_running = 0;
  lua_pushinteger(state, l_running);
  return 1;
}

static int l_event_swap(lua_State* state){
  swap_At = lua_tonumber(state, 1);
  return 1;
}

static luaL_Reg const regFuncs[] = {
  {"quit", l_event_quit},
  {"swap", l_event_swap},
  {NULL,NULL}
};

int l_event_running()
{
  return l_running;
}

int l_event_register(lua_State* state)
{
  l_running = 1;
  event_force_quit = false;
  l_tools_registerModule(state, "event", regFuncs);
  return 0;
}
