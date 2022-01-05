#version 330 core

in vec2 fromVertexTexCoord;
in vec4 fromVertexPos;
uniform sampler2D inputTexture0;
uniform float lightStrength;
uniform vec3 lightPos;
uniform mat4 model;
out vec4 color;

void main(){
	vec4 defaultColor = texture(inputTexture0, fromVertexTexCoord);
	vec3 pixelPos = vec3(fromVertexPos.x, fromVertexPos.y, fromVertexPos.z);
	if (distance(lightPos, pixelPos) < 0.5*lightStrength){
		defaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (defaultColor.a < 0.1 ){
		discard;
	}
	color = defaultColor;
};
