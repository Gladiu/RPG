#pragma once
#include "render/sprites.h"
// Explanation of what player is 

typedef struct player
{
	sprites sprite;
	mat4 view;

}player;

void InitPlayer(player* inputPlayer);
void DrawPlayer(player* inputPlayer);
