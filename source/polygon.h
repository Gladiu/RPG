#pragma once
#include "libs/cglm/mat3.h"

typedef struct polygon
{
	mat3 orientation;
	vec2 position;
	float width;
	float height;
}polygon;

void Move(polygon* inputPolygon, vec2 desiredDirection, float deltaTime);
void Rotate(polygon* inputPolygon, mat3 desiredOrientation, float deltaTime);
bool Collides(polygon* polygon1, polygon polygon2);
