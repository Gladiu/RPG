#include "player.h"
#include <math.h>

void InitPlayer(player* inputPlayer, mat4* projection)
{
	glm_mat4_identity(inputPlayer->view);
	glm_mat4_identity(inputPlayer->model);
	glm_translate(inputPlayer->view, (vec4){0.0f, 0.0f, -5.0f});
	InitSprite(&(inputPlayer->sprite), &inputPlayer->model, projection, &inputPlayer->view, "../source/textures/person.png");

}

void DrawPlayer(player* inputPlayer)
{
	DrawSprite(&(*inputPlayer).sprite);
}

void MoveWithPhysicsPlayer(player *inputPlayer, vec2 movement, float deltaTime, float speedFactor)
{
	// Normalising movement vector 
	double vectorLength = sqrt(movement[0]*movement[0] +movement[1]*movement[1]);
	if (vectorLength != 0){
		movement[0] /= vectorLength;
		movement[1] /= vectorLength;
	}
	
	// Moving hitbox and sprite
	// Sprite is updating becouse it holds pointer to players model matrix
	inputPlayer->model[3][0] += movement[0] * deltaTime * speedFactor;
	inputPlayer->model[3][1] += movement[1] * deltaTime * speedFactor;

	// Since we are moving our player we need to move camera with him
}
