#include "collider.h"
#include <memory.h>

void InitCollider(collider* inputCollider, mat4* projection, mat4* view, char texturePath[])
{
	inputCollider->collisionShape.position[0] = 0;
	inputCollider->collisionShape.position[1] = 0;
	InitSprite(&(inputCollider->sprite), projection, view, texturePath);

}

void DrawCollider(collider* inputCollider)
{
	DrawSprite(&(*inputCollider).sprite);
}

void MoveWithPhysicsCollider(collider *inputCollider, vec2 movement, float deltaTime)
{
	PhysicsMove(&inputCollider->collisionShape, movement, deltaTime);
	// Update model position
	glm_translate(inputCollider->sprite.model, (vec4){movement[0], movement[1], 0.0f, 0.0f});
}
