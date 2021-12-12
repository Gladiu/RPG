#pragma once
#include "../render/sprite.h"
#include "polygon.h"
// Explanation of what collider is 

typedef struct collider
{
	sprite sprite;
	polygon collisionShape;
}collider;

void InitCollider(collider* inputCollider, mat4* projection, mat4* view, char texturePath[]);
void DrawCollider(collider* inputCollider);
void MoveWithPhysicsCollider(collider* inputCollider, vec2 movement, float deltaTime);
