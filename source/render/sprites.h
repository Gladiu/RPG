#pragma once
#include <GLFW/glfw3.h>
#include "../logic/player.h"

typedef struct sprites
{
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint EBO; // Element Buffer Object
	GLuint shaderProgram;
}sprites;


void InitSprites(sprites* inputSprite);

void DrawSprites(sprites* inputSprite, player* inputPlayer);
