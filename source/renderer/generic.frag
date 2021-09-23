#version 330 core
//in vec2 TexCoord;

out vec4 color;

//uniform sampler2D myTexture0;

void main()
{
	color = vec4(0.5f, 0.5f, 0.5f, 1.0f);//texture(myTexture0, vec2(TexCoord.x, 1 - TexCoord.y));
}
