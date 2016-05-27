/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../physics/physics.h"
#include "tools.h"

static struct {
  lua_State *luaState;
} moduleData;


static int l_phyisics_newWorld(lua_State* state) {
  float grav_x = luaL_checknumber(state, 1);
  float grav_y = luaL_checknumber(state, 2);
  int sleep = luaL_checknumber(state, 3); 
  physics_newWorld(grav_x, grav_y, sleep);
  return 1;
}

static luaL_Reg const regFuncs[] = {
  {"newWorld", l_phyisics_newWorld},
  {NULL,NULL}
};

int l_physics_register(lua_State* state) {
  l_tools_registerModule(state, "physics", regFuncs);
  return 0;
}
