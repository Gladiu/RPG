#include "../common.h"

typedef struct sprites
{
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint EBO; // Element Buffer Object
	GLuint shaderProgram;
}sprites;


void InitSprites();

void DrawSprites();
