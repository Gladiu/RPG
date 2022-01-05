#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 fromVertexTexCoord;
out vec4 fromVertexPos;

void main(){
	fromVertexTexCoord = texCoord;
	fromVertexPos = vec4(position.x, position.y, position.z, 1.0f);

	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
}
