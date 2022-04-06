#version 330 core

in vec2 fromVertexTexCoord;
in vec4 fromVertexPos;
in vec3 fromVertexNormal;

uniform sampler2D inputTexture0;
uniform float lightStrength;
uniform vec3 lightPos;
uniform mat4 model;

out vec4 color;

float lookUpTable[16] = float[16]( 0.0, 0.0625, 0.125, 0.1875, 0.25,
					0.3125, 0.375, 0.4375, 0.5, 0.5625,
					0.625, 0.6875, 0.75, 0.8125, 0.875,
					0.9375);
void main(){
	vec4 defaultColor = texture(inputTexture0, fromVertexTexCoord);
	vec4 worldPosition4 = model * fromVertexPos;
	vec3 worldPosition = vec3(worldPosition4.x, worldPosition4.y, worldPosition4.z);
	if (distance(lightPos, worldPosition )< 0.5*lightStrength){
		defaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (defaultColor.a < 0.1 ){
		discard;
	}
	color = defaultColor;
};
