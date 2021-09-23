#pragma once

#include "../common.hpp"
#include <string>

class Texture
{
	public:

	explicit Texture(){};

	// Constructs a texture object from pathToTexture
	bool Init(const std::string &pathToTexture);

	const GLuint GetIdentifier();

	private:
	// OpenGL texture identifier
	GLuint identifier;

	// Width of texture
	int width;

	// Height of texture
	int height;
};

