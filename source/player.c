#include "player.h"

void InitPlayer(player* inputPlayer)
{
	InitSprites(&inputPlayer->sprite);
	inputPlayer->sprite.view = &inputPlayer->view;
}

void DrawPlayer(player* inputPlayer)
{
	DrawSprites(&inputPlayer->sprite);
}
