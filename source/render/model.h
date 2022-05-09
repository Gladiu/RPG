#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/include/cglm/cglm.h"

// Model are 3D models used in world
// I hope you suffer when reading this
// as much as i suffered writing this

// Node is instance of a mesh
typedef struct node{
	// node model matrix is relative to model matrix
	mat4 model;
	unsigned int meshIndex;
}node;

// Primitives correspond to the data required for GPU draw calls
typedef struct primitive{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	unsigned int indiceCount;
	unsigned int materialIndex;
}primitive;

typedef struct material{
	GLuint diffuseIndex;
	GLuint diffuseTexCoord;

	GLuint normalIndex;
	GLuint normalTexCoord;

	GLuint occlusionIndex;
	GLuint occlusionTexCoord;

	GLuint emissiveIndex;
	GLuint emissiveTexCoord;
	vec3 emissiveFactor;

}material;

typedef struct model
{
	// Mesh is array of primitives
	unsigned int *primitiveCount;
	unsigned int meshCount;
	primitive **meshArray;

	unsigned int nodeCount;
	node *nodeArray;

	unsigned int materialCount;
	material *materialArray;

	mat4 model;

	GLuint shaderProgram;
}model;

void InitModel(model* inputModel,const char modelPath[]);

void DrawModel(model* inputModel, mat4 *projection, mat4 *view);

