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

//TODO garbage collector

void physics_init() {

}

//WORLD
void physics_newWorld(physics_World* world, float x, float y, float sleep) { 
  world->space = cpSpaceNew(); //create a new world
  
  //init gravity 
  world->gravity = cpv(x,y);
  cpSpaceSetGravity(world->space, world->gravity); //use the new world that has been created 
 
  cpSpaceSetSleepTimeThreshold(world->space, sleep); //after how many seconds to sleep 
}

void physics_updateWorld(physics_World* world, float dt) {
  cpSpaceStep(world->space, dt);  
}

//BODY
void physics_newBody(physics_World* world, physics_Body* body, float mass, float momentum) {
  body->body = cpBodyNew(mass, momentum);
  cpSpaceAddBody(world->space, body->body);
}

void physics_setBodyPosition(physics_Body* body, float x, float y) {
  cpVect pos = cpv(x,y);
  cpBodySetPosition(body->body, pos);
}

float physics_getBodyX(physics_Body* body) {
 return cpBodyGetPosition(body->body).x; 
}

float physics_getBodyY(physics_Body* body) { 
 return cpBodyGetPosition(body->body).y; 
}
