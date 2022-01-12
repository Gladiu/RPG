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
	// Get texel pos from UV map
	// map texel position to world space
	// add texel pos to integer value of position
	// to get texel pos in world space
	vec3 pixelPos = vec3(fromVertexPos.x, fromVertexPos.y, fromVertexPos.z);
	ivec3 intPixelPos = ivec3(floor(fromVertexPos.x), floor(fromVertexPos.y), floor(fromVertexPos.z));
	vec3 temp = pixelPos-intPixelPos;
	temp *= 16;
	temp = floor(temp); // TODO Declare own round
	temp /= 16;
	if (distance(lightPos, temp+intPixelPos) < 0.5*lightStrength){
		defaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (defaultColor.a < 0.1 ){
		discard;
	}
	color = defaultColor;
};
