#version 330 core

in vec2 fromVertexTexCoord;
uniform sampler2D inputTexture0;
out vec4 color;

void main(){
	vec4 tempColor = texture(inputTexture0, fromVertexTexCoord);
	if(tempColor.a < 0.1 )
	discard;
	color = tempColor;
};
