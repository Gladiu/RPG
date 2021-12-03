#pragma once
#include "render/sprite.h"
// Explanation of what player is 

typedef struct player
{
	sprite sprite;
	mat4* projection;
	mat4 view;
}player;

void InitPlayer(player* inputPlayer, mat4* projection);
void DrawPlayer(player* inputPlayer);
