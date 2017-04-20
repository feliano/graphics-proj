#version 330 core

in vec2 UV;
in vec3 texCoords;
out vec3 color;

uniform samplerCube tex;

void main()  {

	color =  texture(tex, texCoords).rgb;
	
}