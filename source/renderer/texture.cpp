#include "texture.hpp"
#include <SOIL/SOIL.h>

#include <iostream> // std::cerr

#include <fstream> // std::ifstream

bool Texture::Init(const std::string &pathToTexture)
{
	// Checking if file exists
	std::ifstream file(pathToTexture.c_str());
	if (file.good())
	{
		// Loading texture data from memory
		unsigned char* textureData = SOIL_load_image(pathToTexture.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &(this->identifier));
		glBindTexture(GL_TEXTURE_2D, identifier);

		// Setting texture options
		// They wont be changed as all textures are supposed to be pixel-art
		// Best settings for pixel-art are already chosen here
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Passing image data to texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		
		// Probably wont need mipmaps but generating them anyways
		glGenerateMipmap(GL_TEXTURE_2D);

		// Clearing up before leaving scope
		SOIL_free_image_data(textureData);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}
	else
	{
		std::cerr << "COULDNT CREATE TEXTURE!\n";
		return false;
	}
}


const GLuint Texture::GetIdentifier()
{
	return identifier;
}

