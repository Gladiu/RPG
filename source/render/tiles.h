#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Tiles are supposed to be unchanged static images 
// that are viewed at certain angle from camera

typedef struct tiles
{
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint tex0; // Texture index
	GLuint shaderProgram;
	mat4* projection;
	mat4* view;
	mat4 model;
}tiles;


void InitTiles(tiles* inputTiles, mat4* projection, mat4* view, int *map, size_t height, size_t width, char texturePath[]);

void DrawTiles(tiles* inputTiles);
