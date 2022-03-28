#include "player.h"
#include "shape.h"
#include <math.h>

void InitPlayer(player* inputPlayer, mat4* projection)
{
	inputPlayer->lastUpdateTime = 0;
	inputPlayer->currentFrame = 0;
	inputPlayer->totalAnimationFrames = 8;
	inputPlayer->currentState = 0;

	glm_mat4_identity(inputPlayer->view);
	glm_mat4_identity(inputPlayer->model);
	glm_translate(inputPlayer->view, (vec4){0.0f, 0.0f, -5.0f});

	// 4 is number of state for each direction player can be facing
	InitSprite(&(inputPlayer->sprite),4 ,&inputPlayer->model, projection, &inputPlayer->view, "../source/textures/character.png");
	inputPlayer->collisionShape.sides = calloc(1, sizeof(line)*4);
	line lines[4] = {0.0f, 0.0f, 0.0f, 1.0f,
			 0.0f, 1.0f, 1.0f, 1.0f,
			 1.0f, 1.0f, 1.0f, 0.0f,
			 1.0f, 0.0f, 0.0f, 0.0f};

	InitShape(&inputPlayer->collisionShape, (vec2){inputPlayer->model[3][0], inputPlayer->model[3][1]}, 0, 4, lines);

}

void DrawPlayer(player* inputPlayer, double currentTime)
{
	unsigned int newPlayerState;
	// It is by design that north and south
	// orientations are taking priority
	if (sqrt(inputPlayer->velocity[0]*inputPlayer->velocity[0] + inputPlayer->velocity[1]*inputPlayer->velocity[1]) != 0){
		if (inputPlayer->velocity[0] > 0){
			newPlayerState = east;
		}
		else if (inputPlayer->velocity[0] < 0){
			newPlayerState = west;
		}
		if (inputPlayer->velocity[1] > 0){
			newPlayerState = south;
		}
		else if (inputPlayer->velocity[1] < 0){
			newPlayerState = north;
		}

		if (newPlayerState != inputPlayer->currentState){
			inputPlayer->currentFrame = 0;
			inputPlayer->currentState = newPlayerState;

			// To prevent changing frame if it was avalible
			inputPlayer->lastUpdateTime = currentTime;
		}
	}
	
	DrawSprite(&(*inputPlayer).sprite, inputPlayer->currentState, inputPlayer->currentFrame);
	
	if (currentTime - inputPlayer->lastUpdateTime > 0.05f){
		inputPlayer->lastUpdateTime = currentTime;
		inputPlayer->currentFrame  = inputPlayer->currentFrame+1 >= inputPlayer->totalAnimationFrames ? 0 : inputPlayer->currentFrame + 1;
	}

	if (inputPlayer->velocity[0] == 0 && inputPlayer->velocity[1] == 0){
		inputPlayer->currentFrame = 0;
	}
}

void CalculateVelocity(player *inputPlayer, vec2 movement, float deltaTime, float speedFactor)
{
	// Normalising movement vector 
	double vectorLength = sqrt(movement[0]*movement[0] + movement[1]*movement[1]);
	if (vectorLength != 0){
		movement[0] /= vectorLength;
		movement[1] /= vectorLength;
	}
	
	// Moving hitbox and sprite
	// Sprite is updating becouse it holds pointer to players model matrix
	
	inputPlayer->velocity[0] = movement[0] * deltaTime * speedFactor;
	inputPlayer->velocity[1] = movement[1] * deltaTime * speedFactor;
}

void ApplyVelocity(player *inputPlayer){

	inputPlayer->model[3][0] += inputPlayer->velocity[0];
	inputPlayer->model[3][1] += inputPlayer->velocity[1];

	SetPosShape(&inputPlayer->collisionShape, (vec2){inputPlayer->model[3][0], inputPlayer->model[3][1]});

	// Since we are moving our player we need to move camera with him
	inputPlayer->view[3][0] = -inputPlayer->model[3][0];
	inputPlayer->view[3][1] = -inputPlayer->model[3][1];

}
