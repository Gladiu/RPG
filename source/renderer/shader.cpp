#include "shader.hpp"
#include <fstream> //std::fstream
#include <iostream> //std::cerr
#include <sstream>
#include <string>
#include "../common.hpp"

bool Shader::Init(const std::string &vertexPath, const std::string &fragmentPath)
{
	// Opening files, if successfull progress further
	std::fstream vertexFile(vertexPath, std::fstream::in);
	std::fstream fragmentFile(fragmentPath, std::fstream::in);
	if (vertexFile.good() and fragmentFile.good())
	{
		// Getting shader code into proper format
		std::stringstream vertexCodeStream;
		std::stringstream fragmentCodeStream;

		vertexCodeStream << vertexFile.rdbuf();
		fragmentCodeStream << fragmentFile.rdbuf();

		std::string vertexCode;
		std::string fragmentCode;
		
		vertexCode = vertexCodeStream.str();
		fragmentCode = fragmentCodeStream.str();

		const GLchar *vertexSource = vertexCode.c_str();
		const GLchar *fragmentSource = fragmentCode.c_str();
	
		GLint succes;
		GLchar infoLog[512];
		// Creating shaders
		GLuint fragment;
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
		if(!succes)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cerr<<"FATAL. COULDNT COMPILE FRAGMENT SHADER\n"<<infoLog<<"\n";
		}

		GLuint vertex;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
		if(!succes)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr<<"FATAL. COULDNT COMPILE VERTEX SHADER\n"<<infoLog<<"\n";
		}
		
		this->program = glCreateProgram();
		glAttachShader(this->program, vertex);
		glAttachShader(this->program, fragment);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_LINK_STATUS, &succes);
		if(!succes)
		{
			glGetProgramInfoLog(this->program, 512, NULL, infoLog);
			std::cerr<<"FATAL. COULDNT LINK SHADER PROGRAM\n"<<infoLog<<"\n";
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return true;
	}
	else
	{
		if (!vertexFile.good())
			std::cerr<<"FATAL ERROR. COULDNT OPEN VERTEX SHADER CODE\n";
		else
			std::cerr<<"FATAL ERROR. COULDNT OPEN FRAGMENT SHADER CODE\n";
		return false;
	}
}

void Shader::Use()
{
	glUseProgram(this->program);
}
