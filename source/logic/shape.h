#pragma once
#include "../libs/cglm/call.h"
#include <stdbool.h>

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
typedef struct shape
{
	float x;
	float y;
	float angle; 
	float width;
	float height;
}shape;

void Move(shape* inputShape, vec2 desiredDirection);

void Rotate(shape* inputShape, float angle);

bool Collides(shape* shape1, shape shape2);
