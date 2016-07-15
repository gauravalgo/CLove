/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../3rdparty/lua/lauxlib.h"

#include "image.h"
#include "tools.h"
#include "../image/imagedata.h"

static struct {
  int imageDataMT;
  image_ImageData* imageData;
} moduleData;

image_ImageData* l_image_getImageData() {
  return moduleData.imageData;
}

int l_image_newImageData(lua_State* state) {
  moduleData.imageData = (image_ImageData*)lua_newuserdata(state, sizeof(image_ImageData));
  int s1type = lua_type(state, 1);
  if(s1type == LUA_TSTRING) {
      moduleData.imageData->path = lua_tostring(state, 1);
      if(image_ImageData_new_with_filename(moduleData.imageData, lua_tostring(state, 1)) == 0){
          lua_pushstring(state, "Could not load image file: ");
          lua_pushstring(state, lua_tostring(state, 1));
          lua_pushstring(state, ", reason: ");
          lua_pushstring(state, image_error());
          lua_concat(state, 4);
          return lua_error(state);
        }
    } else if(s1type == LUA_TNUMBER && lua_type(state, 2) == LUA_TNUMBER) {
      image_ImageData_new_with_size(moduleData.imageData, lua_tointeger(state, 1), lua_tointeger(state, 2));
    } else {
      lua_pushstring(state, "need filename or size for imagedata");
      return lua_error(state);
    }

  lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.imageDataMT);
  lua_setmetatable(state, -2);

  return 1;
}

static int l_image_ImageData_getWidth(lua_State* state) {
   image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);	
	lua_pushinteger(state, image_ImageData_getWidth(imagedata));
	return 1;
}

static int l_image_ImageData_getHeight(lua_State* state) {
   image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);	
	lua_pushinteger(state, image_ImageData_getHeight(imagedata));
	return 1;
}

static int l_image_ImageData_getDimensions(lua_State* state) {
   image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);	
	lua_pushinteger(state, image_ImageData_getWidth(imagedata));	
	lua_pushinteger(state, image_ImageData_getHeight(imagedata));
	return 2;
}

static int l_image_ImageData_getPixel(lua_State* state) {
	image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
	int x = lua_tointeger(state, 2);
	int y = lua_tointeger(state, 3);
	
	lua_pushinteger(state, image_ImageData_getPixel(imagedata, x, y));
	return 1;
}

static int l_image_gcImageData(lua_State* state) {
  image_ImageData* imagedata = (image_ImageData*)lua_touserdata(state, 1);
  image_ImageData_free(imagedata);
  return 0;
}

static luaL_Reg const regFuncs[] = {
  {"newImageData", l_image_newImageData},
  {NULL, NULL}
};

l_checkTypeFn(l_image_isImageData, moduleData.imageDataMT)
l_toTypeFn(l_image_toImageData, image_ImageData)

static luaL_Reg const imageDataMetatableFuncs[] = {
  {"getWidth", l_image_ImageData_getWidth},
  {"getHeight", l_image_ImageData_getHeight},
  {"getDimensions", l_image_ImageData_getDimensions},
  {"getPixel", l_image_ImageData_getPixel},
  {"__gc", l_image_gcImageData},
  {NULL, NULL}
};

int l_image_register(lua_State* state) {
  l_tools_registerModule(state, "image", regFuncs);

  moduleData.imageDataMT = l_tools_makeTypeMetatable(state, imageDataMetatableFuncs);
  
  return 0;
}
