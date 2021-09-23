#include "sprite.hpp"
#include "../common.hpp"

#include <string> //std::string for passing  paths

Sprite::Sprite()
{
	vertices = new float [12]
	{
		// Vertices        Texture coordinates
		 0.5f,  0.5f, 0.0f,// -0.5f,  0.5f,
		 0.5f, -0.5f, 0.0f,//  0.5f,  0.5f,
		-0.5f, -0.5f, 0.0f,// -0.5f, -0.5f,
		-0.5f,  0.5f, 0.0f//  0.5f, -0.5f
	};

	indices = new GLuint[6]
	{
		0,1,3,
		1,2,3
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)(0));
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), reinterpret_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), reinterpret_cast<GLvoid*>(3));
//	glEnableVertexAttribArray(1);
	
	this->shader.Init(std::string("../source/renderer/generic.vert"), std::string("../source/renderer/generic.frag"));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	this->texture.Init(std::string("textures/test.jpg"));

};

Sprite::~Sprite()
{
	delete vertices;
	delete indices;
}


void Sprite::Draw()
{
	// Using Shader program
	shader.Use();

	glBindVertexArray(VAO);
	/*
	// Enabling texture we want to use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetIdentifier());
	glUniform1i(glGetUniformLocation(shader.GetProgram(),"myTexture0"), 0);
	*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

