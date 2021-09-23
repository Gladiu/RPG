#pragma once

/*
 All of dynamic graphical object should derive from this class
 */
#include "../common.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Sprite
{
	public: 
	// Draw calls to OpenGL
	void Draw();

	// Sprite class constructor
	explicit Sprite();
	
	// Sprite class destructor
	~Sprite();
	protected:
	float *vertices;
	GLuint *indices;
	Texture texture;
	Shader shader;
	GLuint VAO; // Vertex Array Object
	GLuint VBO; // Vertex Buffer Object
	GLuint EBO; // Element Buffer Object

};
