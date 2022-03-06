#include "destructible.h"
#include <math.h>

void InitDestructible(destructible* inputDestructible, mat4* projection, mat4 *view)
{

	inputDestructible->view = view;
	glm_mat4_identity(inputDestructible->model);
	InitSprite(&(inputDestructible->sprite),4 ,&inputDestructible->model, projection, inputDestructible->view, "../source/textures/crate.png");
	inputDestructible->collisionShape.sides = calloc(1, sizeof(line)*4);
	line lines[4] = {0.0f, 0.0f, 0.0f, 1.0f,
			 0.0f, 1.0f, 1.0f, 1.0f,
			 1.0f, 1.0f, 1.0f, 0.0f,
			 1.0f, 0.0f, 0.0f, 0.0f};

	InitShape(&inputDestructible->collisionShape,(vec2){inputDestructible->model[3][0], inputDestructible->model[3][1]} , 0, 4, lines);

}

void DrawDestructible(destructible* inputDestructible, double currentTime)
{
	
	DrawSprite(&(*inputDestructible).sprite, 0, 0);
}

void MoveWithPhysicsDestructible(destructible *inputDestructible, vec2 force, float deltaTime, float speedFactor)
{
	// Normalising force vector 
	double vectorLength = sqrt(force[0]*force[0] +force[1]*force[1]);
	if (vectorLength != 0){
		force[0] /= vectorLength;
		force[1] /= vectorLength;
	}
	
	// TODO calculate stuff
}
