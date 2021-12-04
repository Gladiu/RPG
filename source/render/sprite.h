#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Sprites are dynamic objects that player can interact with 
// they are facing directly camera
// their position is in same cooridnates as tiles but they are rotated around X axis

typedef struct sprite
{
	float x;
	float y;
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint tex0; // Texture index
	GLuint shaderProgram;
	mat4* projection;
	mat4* view;
	mat4 model;
}sprite;


void InitSprite(sprite* inputSprites, mat4* projection, mat4* view);

void DrawSprite(sprite* inputSprites);