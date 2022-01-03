#version 330 core

in vec2 fromVertexTexCoord;
uniform sampler2D inputTexture0;
uniform float frameNumber;
out vec4 color;

void main(){
	vec4 defaultColor = texture(inputTexture0, fromVertexTexCoord);
	if(defaultColor.a < 0.1 ){
		discard;
	}
	color = defaultColor;
};
