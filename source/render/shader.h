#pragma once

const char *genericVertexShader = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"layout (location = 1) in vec2 texCoord;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"out vec2 fromVertexTexCoord;\n"
	"void main()\n"
	"{fromVertexTexCoord = texCoord;\n"
	"gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);}";

const char *genericFragmentShader =
	"#version 330 core\n"
	"in vec2 fromVertexTexCoord;\n"
	"uniform sampler2D inputTexture0;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{color = texture(inputTexture0, fromVertexTexCoord);}";
