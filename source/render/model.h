#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Model are 3D models used in world

typedef struct model
{
	GLuint VAO; // Vertex Array Object
	GLuint EBO; //
	GLuint VBO; // Vertex Buffer Object
	GLuint tex0; // Texture index
	GLuint shaderProgram;
	mat4 model;
}model;


void InitModel(model* inputModel,const char modelPath[],const char texturePath[]);

void DrawModel(model* inputModel, unsigned int currentState, unsigned int currentFrame);
