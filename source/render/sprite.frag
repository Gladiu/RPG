#version 330 core

in vec2 fromVertexTexCoord;
uniform sampler2D inputTexture0;
uniform float frameNumber;
uniform float totalAnimationFrames;
uniform float stateNumber;
uniform float totalStates;
out vec4 color;

void main(){
	ivec2 desiredPixel = ivec2(16*(frameNumber +fromVertexTexCoord.x), 16*(stateNumber+fromVertexTexCoord.y));
	vec4 defaultColor = texelFetch(inputTexture0, desiredPixel, 0);
	if(defaultColor.a < 0.1 ){
		discard;
	}
	color = defaultColor;
};
