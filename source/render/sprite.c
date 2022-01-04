#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "sprite.h"
//#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/cglm/cglm.h"

void InitSprite(sprite* inputSprite, unsigned int totalStates,mat4* model, mat4* projection, mat4* view, char texturePath[])
{

	inputSprite->projection = projection;
	inputSprite->view = view;
	inputSprite->model = model;

	inputSprite->totalStates = totalStates;

	float vertices[30]=
	{
		// Coordinates       Texture Coordinates
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	};
	

	// Enabling and binding buffers buffers
	
	glGenVertexArrays(1, &inputSprite->VAO);
	glGenBuffers(1, &inputSprite->VBO);

	glBindVertexArray(inputSprite->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, inputSprite->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*30, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,5 * sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// Creating Shaders
	
	FILE *f = fopen("../source/render/sprite.frag", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *genericFragmentShader = calloc(1, fsize + 1);
	fread(genericFragmentShader , fsize, 1, f);
	fclose(f);

	GLint succes;
	GLchar infoLog[512];
	GLuint fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, (const GLchar**)&genericFragmentShader, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
	if(!succes)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT COMPILE FRAGMENT SHADER %s\n", infoLog);
		exit(EXIT_FAILURE);
	}

	f = fopen("../source/render/sprite.vert", "rb");
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *genericVertexShader = calloc(1, fsize + 1);
	fread(genericVertexShader , fsize, 1, f);
	fclose(f);

	GLuint vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, (const GLchar**)&genericVertexShader, NULL);
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

	// Generating Textures
	int textureWidth, textureHeight;
	unsigned char *image = SOIL_load_image(texturePath, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
	glGenTextures(1, &inputSprite->tex0);
	glBindTexture(GL_TEXTURE_2D, inputSprite->tex0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	free(genericFragmentShader);
	free(genericVertexShader);
}

void DrawSprite(sprite* inputSprite, unsigned int currentState, unsigned int currentFrame)
{
	glUseProgram(inputSprite->shaderProgram);
	
	GLint modelLoc = glGetUniformLocation(inputSprite->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)inputSprite->model);

	GLint viewLoc = glGetUniformLocation(inputSprite->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)inputSprite->view);

	GLint projectionLoc = glGetUniformLocation(inputSprite->shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)inputSprite->projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputSprite->tex0);
	glUniform1i(glGetUniformLocation(inputSprite->shaderProgram,"inputTexture0"), 0);

	GLint animationFrameLoc = glGetUniformLocation(inputSprite->shaderProgram, "frameNumber");
	glUniform1f(animationFrameLoc, (float)currentFrame);

	GLint totalAnimationFramesLoc = glGetUniformLocation(inputSprite->shaderProgram, "totalAnimationFrames");
	glUniform1f(totalAnimationFramesLoc, 8.0f); // TODO change to be variable

	GLint stateNumberLoc = glGetUniformLocation(inputSprite->shaderProgram, "stateNumber");
	glUniform1f(stateNumberLoc, (float)currentState);

	GLint totalStatesLoc = glGetUniformLocation(inputSprite->shaderProgram, "totalStates");
	glUniform1f(totalStatesLoc, (float)inputSprite->totalStates);

	glBindVertexArray(inputSprite->VAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}
