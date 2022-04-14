#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Model are 3D models used in world
// I hope you suffer when reading this
// as much as i suffered writing this

// Node is instance of a mesh
typedef struct node{
	mat4 model;
	unsigned int meshIndex;
}node;

// Primitives correspond to the data required for GPU draw calls
typedef struct primitive{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	unsigned int indiceCount;
}primitive;

typedef struct model
{
	unsigned int *primitiveCount;
	unsigned int meshCount;
	// Mesh is array of primitives
	primitive **meshArray;
	GLuint tex0; // Texture index
	GLuint shaderProgram;
	mat4 model;
}model;


void InitModel(model* inputModel,const char modelPath[],const char texturePath[]);

void DrawModel(model* inputModel, mat4 *projection, mat4 *view);
