/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "tools.h"
#include "math.h"
#include "../math/minmax.h"
#include "../math/random.h"

int l_math_random(lua_State* state) {
  float lo = 0.0f, hi = 1.0f;
  int t = lua_gettop(state);
  switch(t) {
  case 1:
    hi = l_tools_toNumberOrError(state, 1);
    break;

  case 2:
    lo = l_tools_toNumberOrError(state, 1);
    hi = l_tools_toNumberOrError(state, 2);
    break;
  }

  lua_pushnumber(state, math_random(lo, hi));
  return 1;
}

int l_math_max(lua_State* state) {
  float x = l_tools_toNumberOrError(state, 1);
  float y = l_tools_toNumberOrError(state, 2);
  float result = max(x,y);
  lua_pushnumber(state, result);
  return 1;
}

int l_math_min(lua_State* state) {
  float x = l_tools_toNumberOrError(state, 1);
  float y = l_tools_toNumberOrError(state, 2);
  float result = min(x,y);
  lua_pushnumber(state, result);
  return 1;
}

int l_math_clamp(lua_State* state) {
  float a = l_tools_toNumberOrError(state, 1);
  float x = l_tools_toNumberOrError(state, 1);
  float y = l_tools_toNumberOrError(state, 2);
  float result = clamp(a, x, y);
  lua_pushnumber(state, result);
  return 1;
}

static luaL_Reg const mathFreeFuncs[] = {
  {"random", l_math_random},
  {"max", l_math_max},
  {"min", l_math_min},
  {"clamp", l_math_clamp},
  {NULL, NULL}
};

void l_math_register(lua_State* state) {
  l_tools_registerModule(state, "math", mathFreeFuncs);
}
