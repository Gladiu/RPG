#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Sprites are dynamic objects 

typedef struct sprite
{
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint tex0; // Texture index
	GLuint shaderProgram;
	mat4* projection;
	mat4* view;
	mat4* model;
	unsigned int totalStates;
}sprite;


void InitSprite(sprite* inputSprite, unsigned int totalStates,mat4* model, mat4* projection, mat4* view, char texturePath[]);

void DrawSprite(sprite* inputSprite, unsigned int currentState, unsigned int currentFrame);
