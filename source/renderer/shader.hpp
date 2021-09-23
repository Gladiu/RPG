#pragma once

#include "../common.hpp"
#include <string>

class Shader
{
	public:

		// Initialize Shader with proper paths
		bool Init(const std::string &vertexPath, const std::string &fragmentPath);

		// Use shader programs
		void Use();

		explicit Shader(){};				

		GLuint GetProgram(){return program;};
	private:
		GLuint program;
};
