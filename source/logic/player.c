#include "player.h"
#include "collider.h"
#include <math.h>

void InitPlayer(player* inputPlayer, mat4* projection)
{
	glm_mat4_identity(inputPlayer->view);
	glm_translate(inputPlayer->view, (vec4){0.0f, 0.0f, -5.0f});
	InitSprite(&(inputPlayer->sprite), projection, &inputPlayer->view, "../source/textures/person.png");

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
	movement[0] *= speedFactor;
	movement[1] *= speedFactor;
	// Moving hitbox and sprite
	// Since we are moving player we need to move camera with him
	MoveSprite(&inputPlayer->sprite, movement, deltaTime);
}
