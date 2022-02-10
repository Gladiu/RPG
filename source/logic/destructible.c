#include "destructible.h"
#include <math.h>

void InitDestructible(destructible* inputDestructible, mat4* projection, mat4 *view)
{

	inputDestructible->view = view;
	glm_mat4_identity(inputDestructible->model);
	InitSprite(&(inputDestructible->sprite),4 ,&inputDestructible->model, projection, inputDestructible->view, "../source/textures/crate.png");

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
