#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "tiles.h"
#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/cglm/cglm.h"

void InitTiles(tiles* inputTiles, int *map, size_t height, size_t width)
{
	
	float tileVertices[30]=
	{
		// Coordinates       Texture Coordinates
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	};
	
	// Creating mesh based on map input to draw entire tilemap at once
	// if it creates a lot of troubles can later divide it into smaller meshes
	float *vertices = malloc(30*(width)*(height)*sizeof(float));
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int tileIndex = y * width + x;
			if(map[tileIndex] > 0) // Can later use map[tileIndex] to determine texture
			{
				for(int i = 0; i < 6; i++)
				{	
					for(int j = 0; j < 5; j++)
					{
						int vertexIndex = i*5+j;
						if(j == 0)
							vertices[tileIndex*30+vertexIndex] = (tileVertices[vertexIndex] + (float)x);
						if(j == 1)
							vertices[tileIndex*30+vertexIndex] = (tileVertices[vertexIndex] + (float)y);
						if(j > 1) 
							vertices[tileIndex*30+vertexIndex] = (tileVertices[vertexIndex]);

					}
				}
			}
		}
	}

	// Enabling and binding buffers buffers
	
	glGenVertexArrays(1, &inputTiles->VAO);
	glGenBuffers(1, &inputTiles->VBO);

	glBindVertexArray(inputTiles->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, inputTiles->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*30*width*height, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,5 * sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
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
	
	inputTiles->shaderProgram = glCreateProgram();
	glAttachShader(inputTiles->shaderProgram, vertex);
	glAttachShader(inputTiles->shaderProgram, fragment);
	glLinkProgram(inputTiles->shaderProgram);
	glGetProgramiv(inputTiles->shaderProgram, GL_LINK_STATUS, &succes);
	if(!succes)
	{
		glGetProgramInfoLog(inputTiles->shaderProgram, 512, NULL, infoLog);
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
	unsigned char *image = SOIL_load_image("/home/gladiu/Programming/rpg/source/textures/tile.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &inputTiles->tex0);
	glBindTexture(GL_TEXTURE_2D, inputTiles->tex0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	free(vertices);
}

void DrawTiles(tiles* inputTile)
{
	glUseProgram(inputTile->shaderProgram);
	
	mat4 model;
	glm_mat4_identity(model);
	GLint modelLoc = glGetUniformLocation(inputTile->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)GLM_MAT4_IDENTITY);

	GLint viewLoc = glGetUniformLocation(inputTile->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)inputTile->view);


	GLint projectionLoc = glGetUniformLocation(inputTile->shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)inputTile->projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputTile->tex0);
	glUniform1i(glGetUniformLocation(inputTile->shaderProgram,"inputTexture0"), 0);


	glBindVertexArray(inputTile->VAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 6*16);

	glBindVertexArray(0);
}
