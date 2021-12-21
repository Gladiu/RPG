#pragma once
#include "../libs/cglm/call.h"
#include <stdbool.h>
#include <stdlib.h>

// Used for collision check and physics

typedef struct shape
{
	vec2 position;
	int pointCount;
	// All points are relative to position vector
	vec2 *pointsArray;
}shape;

void InitShape(shape inputShape, vec2 position, float angle, int pointCount, vec2 pointsArray[3]);

void SetPosShape(shape* inputShape, vec2 desiredPosition);

void SetAngleShape(shape* inputShape, float angle);

bool Collides(shape* shape1, shape shape2);

