#include "polygon.h"


void PhysicsMove(polygon* inputPolygon, vec2 desiredDirection, float deltaTime)
{
	inputPolygon->position[0] += desiredDirection[0] * deltaTime;
	inputPolygon->position[1] += desiredDirection[1] * deltaTime;
}
