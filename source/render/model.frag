#version 330 core

in vec2 fromVertexTexCoord;
in vec4 fromVertexPos;
in vec3 fromVertexNormal;

uniform sampler2D inputTexture0;
uniform float lightStrength;
uniform vec3 lightPos;
uniform mat4 model;

out vec4 color;

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
