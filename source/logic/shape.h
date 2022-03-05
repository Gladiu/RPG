#pragma once
#include "../libs/cglm/call.h"
#include <stdbool.h>
#include <stdlib.h>

// Used for collision check and physics
typedef struct line
{
	vec2 start;
	vec2 end;
}line;

typedef struct shape
{
	vec2 position;
	int sidesCount;
	// All points are relative to position vector
	line *sides;
}shape;

void InitShape(shape* inputShape, vec2 position, float angle, int sidesCount, line sides[sidesCount]);

void SetPosShape(shape* inputShape, vec2 desiredPosition);

void SetAngleShape(shape* inputShape, float angle);

bool Collides(shape* shape1, shape* shape2);

