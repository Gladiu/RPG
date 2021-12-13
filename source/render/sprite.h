#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Sprites are dynamic objects that player can interact with 
// they are facing directly camera
// their position is in same cooridnates as tiles but they are rotated around X axis

typedef struct sprite
{
	vec2 position;
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint tex0; // Texture index
	GLuint shaderProgram;
	mat4* projection;
	mat4* view;
	mat4 model;
}sprite;


void InitSprite(sprite* inputSprite, mat4* projection, mat4* view, char texturePath[]);

void DrawSprite(sprite* inputSprite);

void SetPositionSprite(sprite* inputSprite, vec2 desiredPosition);
