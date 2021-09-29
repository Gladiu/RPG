#pragma once
#include <GLFW/glfw3.h>

typedef struct tiles
{
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint tex0; // Texture index
	GLuint shaderProgram;
}tiles;


void InitTiles(tiles* inputTiles, int *map, size_t height, size_t width);

void DrawTiles(tiles* inputTiles);
