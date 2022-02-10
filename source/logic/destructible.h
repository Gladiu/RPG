#pragma once
#include <stdbool.h>
#include "../render/sprite.h"
#include "shape.h"


typedef struct destructible
{
	sprite sprite;
	shape collisionShape;
	mat4 *view;
	mat4 model;
	vec2 velocity;
}destructible;

void InitDestructible(destructible* inputDestructible, mat4* projection, mat4 *view);

void DrawDestructible(destructible* inputDestructible, double currentTime);

void MoveWithPhysicsDestructible(destructible* inputDestructible, vec2 movement, float deltaTime, float speedFactor);
