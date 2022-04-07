#define CGLTF_IMPLEMENTATION
#include "../libs/cgltf/cgltf.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/cglm/cglm.h"

void InitModel(model *inputModel, const char modelPath[], const char texturePath[]){

	// Loading Model from file
	cgltf_options options = {0};
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, modelPath, &data);


	// Enabling and binding buffers buffers
	float *vertices;
	for ( int meshIndex = 0; meshIndex< data->meshes_count; meshIndex++){
		for
	}


	
	glGenVertexArrays(1, &inputModel->VAO);
	glGenBuffers(1, &inputModel->VBO);
	glGenBuffers(1, &inputModel->EBO);

	glBindVertexArray(inputModel->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, inputModel->VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*30, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,5 * sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// Creating Shaders
	
	FILE *f = fopen("../source/render/generic.frag", "rb");
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

	f = fopen("../source/render/generic.vert", "rb");
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
	
	inputModel->shaderProgram = glCreateProgram();
	glAttachShader(inputModel->shaderProgram, vertex);
	glAttachShader(inputModel->shaderProgram, fragment);
	glLinkProgram(inputModel->shaderProgram);
	glGetProgramiv(inputModel->shaderProgram, GL_LINK_STATUS, &succes);
	if(!succes)
	{
		glGetProgramInfoLog(inputModel->shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT LINK SHADER PROGRAM %s\n", infoLog);
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// Disabling buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// TODO Texture support
	if ( 0 ) {
		int textureWidth, textureHeight;
		unsigned char *image = SOIL_load_image(texturePath, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
		glGenTextures(1, &inputModel->tex0);
		glBindTexture(GL_TEXTURE_2D, inputModel->tex0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
 	}
	free(genericFragmentShader);
	free(genericVertexShader);
	cgltf_free(data);
}

void DrawModel(model* inputModel, unsigned int currentState, unsigned int currentFrame)
{
	glUseProgram(inputModel->shaderProgram);
	
	GLint modelLoc = glGetUniformLocation(inputModel->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)inputModel->model);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputModel->tex0);
	glUniform1i(glGetUniformLocation(inputModel->shaderProgram,"inputTexture0"), 0);

	GLint animationFrameLoc = glGetUniformLocation(inputModel->shaderProgram, "frameNumber");
	glUniform1f(animationFrameLoc, (float)currentFrame);

	GLint totalAnimationFramesLoc = glGetUniformLocation(inputModel->shaderProgram, "totalAnimationFrames");
	glUniform1f(totalAnimationFramesLoc, 8.0f); // TODO change to be variable

	GLint stateNumberLoc = glGetUniformLocation(inputModel->shaderProgram, "stateNumber");
	glUniform1f(stateNumberLoc, (float)currentState);

	glBindVertexArray(inputModel->VAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}
