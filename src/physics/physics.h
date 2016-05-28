/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once 
#include "../3rdparty/chipmunk/include/chipmunk/chipmunk.h"

typedef struct {
  cpBody* body;
  cpVect position;
  cpVect velocity;
  float mass;
}physics_Body;

typedef struct {

}physics_Shape;

typedef struct { 
  cpSpace* space;
  cpVect gravity;
}physics_World;

void physics_init();

//scene functions
void physics_newWorld(physics_World* world, float x, float y, float sleep);
void physics_setIterations(int number);
void physics_updateWorld(physics_World* world, float dt);

//shape functions
void physics_newBody(physics_World* world, physics_Body* body, float mass, float momentum);
void physics_setBodyPosition(physics_Body* body, float x, float y);
float physics_getBodyX(physics_Body* body);
float physics_getBodyY(physics_Body* body);
