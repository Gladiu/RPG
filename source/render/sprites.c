#include "stdio.h"
#include "stdlib.h"
#include "sprites.h"
#include "shader.h"
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

	// Enabling and binding buffers buffers
	
	glGenVertexArrays(1, &inputSprite->VAO);
	glGenBuffers(1, &inputSprite->VBO);
	glGenBuffers(1, &inputSprite->EBO);

	glBindVertexArray(inputSprite->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, inputSprite->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inputSprite->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	// Opening files, if successfull progress further
	FILE *vertexFile;
	FILE *fragmentFile;
	vertexFile = fopen("generic.vert", "r");
	fragmentFile = fopen("generic.frag", "r");

	if (0)//vertexFile == NULL || fragmentFile == NULL)
	{
		if (vertexFile == NULL)
			fprintf(stderr, "FATAL ERROR. COULDNT OPEN VERTEX SHADER CODE\n");
		else
			fprintf(stderr, "FATAL ERROR. COULDNT OPEN FRAGMENT SHADER CODE\n");
	}
	else
	{
		if(0)
		{
			// Reading fragment shader code
			fseek(fragmentFile, 0, SEEK_END);
			long length = ftell(fragmentFile);
			fseek(fragmentFile, 0, SEEK_SET);
			char * fragmentSource;
			fragmentSource = malloc(length);
			fread(fragmentSource, 1, length, fragmentFile );

			// Reading vertex shader code
			fseek(vertexFile, 0, SEEK_END);
			length = ftell(vertexFile);
			fseek(vertexFile, 0, SEEK_SET);
			char * vertexSource;
			vertexSource = malloc(length);
			fread(vertexSource, 1, length, vertexFile );
		}
	

		GLint succes;
		GLchar infoLog[512];
		// Creating shaders
		GLuint fragment;
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		//glShaderSource(fragment, 1, &fragmentSource, NULL);
		glShaderSource(fragment, 1, &genericFragmentShader, NULL);

		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
		if(!succes)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			fprintf(stderr, "FATAL. COULDNT COMPILE FRAGMENT SHADER %s\n", infoLog);
		}

		GLuint vertex;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		//glShaderSource(vertex, 1, &vertexSource, NULL);
		glShaderSource(vertex, 1, &genericVertexShader, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
		if(!succes)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			fprintf(stderr, "FATAL. COULDNT COMPILE VERTEX SHADER %s\n", infoLog);
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
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		//fclose(vertexFile);
		//fclose(fragmentFile);
	}



	// Disabling buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void DrawSprites(sprites* inputSprite)
{
	glUseProgram(inputSprite->shaderProgram);

	glBindVertexArray(inputSprite->VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
