#pragma once
#include "../libs/cglm/call.h"

// Used for collision check and physics
// Might later implement other polygons than square
// Below you can see graphical representation on how 
// that looks in 2D space. I find this way most intuitive for this application
// not necessairly the best for everything and might change
// 
//  Position point
//  |
//  |                         angle rotation direction
//  |                             ----
//  |                                ||
//  |                                ||
//  |                                \/
// \ /     <-width->
//  .______________________.
//  |                      |
//  |                      | /\
//  |                      | h
//  |                      | e
//  |                      | i
//  |                      | g
//  |                      | h
//  |                      | t
//  |                      | \/
//  |                      |
//  .______________________.
//
//
typedef struct polygon
{
	vec2 position;
	float angle; 
	float width;
	float height;
}polygon;

void PhysicsMove(polygon* inputPolygon, vec2 desiredDirection, float deltaTime);

void PhysicsRotate(polygon* inputPolygon, mat3 desiredOrientation, float deltaTime);

bool Collides(polygon* polygon1, polygon polygon2);
