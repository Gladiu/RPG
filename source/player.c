#include "player.h"
#include <memory.h>

void InitPlayer(player* inputPlayer, mat4* projection)
{
	inputPlayer->collisionShape.position[0] = 0;
	inputPlayer->collisionShape.position[1] = 0;
	glm_mat4_identity(inputPlayer->view);
	glm_translate(inputPlayer->view, (vec4){0.0f, 0.0f, -5.0f});
	InitSprite(&(inputPlayer->sprite), projection, &inputPlayer->view);

}

void DrawPlayer(player* inputPlayer)
{
	DrawSprite(&(*inputPlayer).sprite);
}

void MoveWithPhysicsPlayer(player *inputPlayer, vec2 movement, float deltaTime)
{
	PhysicsMove(&inputPlayer->collisionShape, movement, deltaTime);
	// Update model position
	glm_translate(inputPlayer->sprite.model, (vec4){movement[0], movement[1], 0.0f, 0.0f});
	// Update view matrix after updating collisionShape
	// view transform are with -
	glm_translate(inputPlayer->view,(vec4){-movement[0], -movement[1], 0.0f, 0.0f});
	
}
