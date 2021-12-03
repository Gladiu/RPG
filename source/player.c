#include "player.h"
#include <memory.h>

void InitPlayer(player* inputPlayer, mat4* projection)
{
	inputPlayer->projection = projection;
	glm_mat4_identity(inputPlayer->view);
	glm_translate(inputPlayer->view, (vec4){0.0f, 0.0f, -5.0f});
	InitSprite(&inputPlayer->sprite, projection, &inputPlayer->view);

}

void DrawPlayer(player* inputPlayer)
{
	DrawSprite(&inputPlayer->sprite);
}
