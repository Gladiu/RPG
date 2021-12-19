#pragma once
#include <stdbool.h>
#include "../render/sprite.h"
#include "shape.h"
// Explanation of what player is 

typedef struct player
{
	sprite sprite;
	shape collisionShape;
	mat4 view;
	mat4 model;
}player;

void InitPlayer(player* inputPlayer, mat4* projection);

void DrawPlayer(player* inputPlayer);

void MoveWithPhysicsPlayer(player* inputPlayer, vec2 movement, float deltaTime, float speedFactor);
