#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float frameNumber;
uniform float totalAnimationFrames;
uniform float stateNumber;
uniform float totalStates;
out vec2 fromVertexTexCoord;

void main(){
	fromVertexTexCoord = vec2((frameNumber + texCoord.x )/totalAnimationFrames,
			(stateNumber + texCoord.y)/totalStates);
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
}
