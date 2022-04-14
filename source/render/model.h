#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Model are 3D models used in world

typedef struct mesh{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	unsigned int indiceCount;
}mesh;
typedef struct model
{
	unsigned int meshCount;
	mesh *meshArray;
	GLuint tex0; // Texture index
	GLuint shaderProgram;
	mat4 model;
}model;


void InitModel(model* inputModel,const char modelPath[],const char texturePath[]);

void DrawModel(model* inputModel, mat4 *projection, mat4 *view);
