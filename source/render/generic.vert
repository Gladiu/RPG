#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float frameNumber;
uniform float totalAnimationFrames;
out vec2 fromVertexTexCoord;

void main(){
	fromVertexTexCoord = vec2(frameNumber/totalAnimationFrames + texCoord.x/totalAnimationFrames, texCoord.y);
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0f);
}
