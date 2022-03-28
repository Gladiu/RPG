#pragma once
#include <stdbool.h>
#include "../render/sprite.h"
#include "shape.h"

// Current states that are passed to sprite shader.
// Their order is bound to what row is the state
// in sprite sheet
enum playerStates{south, north, west, east};

typedef struct player
{
	sprite sprite;
	shape collisionShape;
	mat4 view;
	mat4 model;
	vec2 velocity;
	double lastUpdateTime;
	unsigned int currentFrame;
	unsigned int totalAnimationFrames;
	unsigned int currentState;
}player;

void InitPlayer(player* inputPlayer, mat4* projection);

void DrawPlayer(player* inputPlayer, double currentTime);

void CalculateVelocity(player* inputPlayer, vec2 movement, float deltaTime, float speedFactor);

void ApplyVelocity(player* inputPlayer);
