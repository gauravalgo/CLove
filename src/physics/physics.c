/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdio.h>
#include "physics.h"
#include "../3rdparty/chipmunk/include/chipmunk/chipmunk.h"

static struct { 
  cpVect gravity;
  cpSpace* space;
  cpShape* shape;
  cpBody* body;
  int sleep;
    
}moduleData;

void physics_newWorld(float x, float y, int sleep) { 
  
  moduleData.space = cpSpaceNew(); //create a new world
  
  moduleData.gravity = cpv(x, y); //init gravity 
  cpSpaceSetGravity(moduleData.space, moduleData.gravity); //use the new world that has been created 
  
}
