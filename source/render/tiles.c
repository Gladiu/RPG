#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "tiles.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../libs/cglm/cglm.h"

void InitTiles(tiles* inputTiles, bool randomTiles, int *map, size_t height, size_t width, const char texturePath[]){


	// TODO its Debug only
	// Setting animation proprieties
	inputTiles->currentFrame = 0;
	inputTiles->totalAnimationFrames = 1;
	inputTiles->lastUpdateTime = 0.0f;

	// Loading texture image here to get texture size to know how many tiles are there
	int textureWidth, textureHeight;
	unsigned char *image = SOIL_load_image(texturePath, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	vec2 maxTextureTileIndex;
	maxTextureTileIndex[0] = (int)(textureWidth)/16;
	maxTextureTileIndex[1] = (int)(textureHeight)/16;
	
	// Generating seed for random tile picking
	srand(time(0));

	glm_mat4_identity(inputTiles->model);
	float tileVertices[48]=
	{
		// Coordinates    UV          Normal
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};
	
	// Creating mesh based on map input to draw entire tilemap at once
	float *vertices = malloc(48*(width)*(height)*sizeof(float));
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){

			int tileIndex = y * width + x;
			if (randomTiles){
				int randomIndex = random() % (int)(maxTextureTileIndex[0] * maxTextureTileIndex[1] ) + 1;
				map[tileIndex] = map[tileIndex] != 0 ? randomIndex : 0;
			}
			if (map[tileIndex] > 0){
				inputTiles->tileCount++;
				for(int i = 0; i < 6; i++){	
					for(int j = 0; j < 8; j++){

						int vertexIndex = i*8+j;
						if (j == 0){
							vertices[tileIndex*48+vertexIndex] = (tileVertices[vertexIndex] + (float)x);
						}
						if (j == 1){
							vertices[tileIndex*48+vertexIndex] = (tileVertices[vertexIndex] + (float)y);
						}
						if (j == 2 || j > 4){
							vertices[tileIndex*48+vertexIndex] = (tileVertices[vertexIndex]);
						}
						if (j == 3){
							vertices[tileIndex*48+vertexIndex] = (tileVertices[vertexIndex]/maxTextureTileIndex[0]);
							vertices[tileIndex*48+vertexIndex] += (map[tileIndex] % (int)(maxTextureTileIndex[0]) - 1.0f)/maxTextureTileIndex[0];
						}
						if (j == 4){
							vertices[tileIndex*48+vertexIndex] = (tileVertices[vertexIndex]/maxTextureTileIndex[1]);
							vertices[tileIndex*48+vertexIndex] += ((map[tileIndex]-1) / (int)(maxTextureTileIndex[0] ))/maxTextureTileIndex[1];

						}

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*48*width*height, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(5*sizeof(float)));
	glEnableVertexAttribArray(2);
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

	free(genericVertexShader);
	free(genericFragmentShader);
	free(vertices);
}

void DrawTiles(tiles* inputTile, mat4* projection, mat4* view, double currentTime){

	glUseProgram(inputTile->shaderProgram);

	GLint lightPosLoc = glGetUniformLocation(inputTile->shaderProgram, "lightPos");
	//glUniform3f(lightPosLoc, inputLight->position[0], inputLight->position[1], inputLight->position[2]);

	GLint lightStrengthLoc = glGetUniformLocation(inputTile->shaderProgram, "lightStrength");
	//glUniform1f(lightStrengthLoc, inputLight->strength);
	
	GLint modelLoc = glGetUniformLocation(inputTile->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)inputTile->model );

	GLint viewLoc = glGetUniformLocation(inputTile->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

	GLint projectionLoc = glGetUniformLocation(inputTile->shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputTile->tex0);
	glUniform1i(glGetUniformLocation(inputTile->shaderProgram,"inputTexture0"), 0);

	GLint animationFrameLoc = glGetUniformLocation(inputTile->shaderProgram, "frameNumber");
	glUniform1f(animationFrameLoc, inputTile->currentFrame);

	GLint totalAnimationFramesLoc = glGetUniformLocation(inputTile->shaderProgram, "totalAnimationFrames");
	glUniform1f(totalAnimationFramesLoc, inputTile->totalAnimationFrames);
	
	glBindVertexArray(inputTile->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6*inputTile->tileCount);
	glBindVertexArray(0);

	if (currentTime - inputTile->lastUpdateTime > 0.50f){
		inputTile->lastUpdateTime = currentTime;
		inputTile->currentFrame  = inputTile->currentFrame+1 >= inputTile->totalAnimationFrames ? 0 : inputTile->currentFrame + 1;
	}
}
