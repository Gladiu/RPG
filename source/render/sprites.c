#include "sprites.h"
#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../common.h"
#include "../libs/cglm/cglm.h"

void InitSprites(sprites* inputSprite)
{
	
	float vertices[20]=
	{
		// Coordinates    Texture Coordinates
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, -1.0f,
		-0.5f, -0.5f, 0.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f, -1.0f, 1.0f
	};

	GLuint indices[6]=
	{
		0,1,3,
		1,2,3
	};

	// Enabling and binding buffers buffers
	
	glGenVertexArrays(1, &inputSprite->VAO);
	glGenBuffers(1, &inputSprite->VBO);
	glGenBuffers(1, &inputSprite->EBO);

	glBindVertexArray(inputSprite->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, inputSprite->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*20, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inputSprite->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	// Creating Shaders

	GLint succes;
	GLchar infoLog[512];
	GLuint fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &genericFragmentShader, NULL);

	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
	if(!succes)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT COMPILE FRAGMENT SHADER %s\n", infoLog);
		exit(EXIT_FAILURE);
	}

	GLuint vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &genericVertexShader, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
	if(!succes)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT COMPILE VERTEX SHADER %s\n", infoLog);
		exit(EXIT_FAILURE);
	}
	
	inputSprite->shaderProgram = glCreateProgram();
	glAttachShader(inputSprite->shaderProgram, vertex);
	glAttachShader(inputSprite->shaderProgram, fragment);
	glLinkProgram(inputSprite->shaderProgram);
	glGetProgramiv(inputSprite->shaderProgram, GL_LINK_STATUS, &succes);
	if(!succes)
	{
		glGetProgramInfoLog(inputSprite->shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT LINK SHADER PROGRAM %s\n", infoLog);
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);



	// Disabling buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void DrawSprites(sprites* inputSprite)
{
	glUseProgram(inputSprite->shaderProgram);
	
	mat4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	vec4 axis = {0,1,0,0};
	//glm_rotate(transform, 45.0f, axis);

	GLint transformLoc = glGetUniformLocation(inputSprite->shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)transform);

	glBindVertexArray(inputSprite->VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
