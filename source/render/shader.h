#pragma once

const char *genericFragmentShader =
	"#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{color = vec4(0.5f, 0.5f, 0.5f, 0.5f);}";

const char *genericVertexShader = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{gl_Position = vec4(position.x, position.y, position.z, 1.0f);}";
