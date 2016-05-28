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
  int physicsDataMT;
} moduleData;


//WORLD
static int l_physics_newWorld(lua_State* state) {
  float grav_x = luaL_checknumber(state, 1);
  float grav_y = luaL_checknumber(state, 2);
  int sleep = luaL_checknumber(state, 3);
  
  physics_World* world = lua_newuserdata(state, sizeof(physics_World));
  
  physics_newWorld(world, grav_x, grav_y, sleep);
  return 1;
}

static int l_physics_update(lua_State* state) { 
  physics_World* world = (physics_World*)lua_touserdata(state, 1);
  float dt = luaL_checknumber(state, 2);
  physics_updateWorld(world, dt);
  return 1;
}

//BODY
static int l_physics_newBody(lua_State *state) {
  physics_Body *body = lua_newuserdata(state, sizeof(physics_Body));  
  
  physics_World* world = (physics_World*)lua_touserdata(state, 1);
  
  float mass = luaL_checknumber(state, 2);
  float momentum = luaL_checknumber(state, 3); 
  physics_newBody(world, body, mass, momentum); 
 
  lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.physicsDataMT);
  lua_setmetatable(state, -2);
  return 1;
}

static int l_physics_body_setPosition(lua_State* state) {
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  float x = luaL_checknumber(state, 2);
  float y = luaL_checknumber(state, 3);
  physics_setBodyPosition(body, x, y);
  return 1;
}

static int l_physics_body_getX(lua_State* state) { 
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  lua_pushnumber(state,physics_getBodyX(body));
  return 1;
}

static int l_physics_body_getY(lua_State* state) { 
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  lua_pushnumber(state,physics_getBodyY(body));
  return 1;
}

static luaL_Reg const SourceMetatableFuncs[] = {
  //{"__gc", l_audio_SourceCommon_free},
  {"setPosition", l_physics_body_setPosition},
  {"getX", l_physics_body_getX},
  {"getY", l_physics_body_getY},
  {NULL, NULL}
};

static luaL_Reg const regFuncs[] = {
  {"newWorld", l_physics_newWorld},
  {"newBody", l_physics_newBody},
  {"update", l_physics_update},

  {NULL,NULL}
};

int l_physics_register(lua_State* state) {
  l_tools_registerModule(state, "physics", regFuncs);
   moduleData.physicsDataMT = l_tools_makeTypeMetatable(state, SourceMetatableFuncs);
  
  return 0;
}
