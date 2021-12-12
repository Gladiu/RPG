#pragma once
#include "../render/sprite.h"
#include "polygon.h"
// Explanation of what player is 

typedef struct player
{
	sprite sprite;
	polygon collisionShape;
	mat4 view;
}player;

void InitPlayer(player* inputPlayer, mat4* projection);
void DrawPlayer(player* inputPlayer);
void MoveWithPhysicsPlayer(player* inputPlayer, vec2 movement, float deltaTime);
