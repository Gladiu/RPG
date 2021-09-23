#include "sprites.h"
#include "../common.h"

void InitSprites(sprites* inputSprite)
{
	
	float vertices[12]=
	{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	GLuint indices[6]=
	{
		0,1,3,
		1,2,3
	};

	glGenVertexArrays(1, &inputSprite->VAO);
	glGenBuffers(1, &inputSprite->VBO);
	glGenBuffers(1, &inputSprite->EBO);

	glBindVertexArray(inputSprite->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, inputSprite->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inputSprite->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*12, indices, GL_STATIC_DRAW);

	glVertexAttribPointer

}

void DrawSprites()
{

}
