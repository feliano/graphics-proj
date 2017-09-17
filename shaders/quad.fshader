#version 330 core

out vec4 color;
in vec2 UV;

uniform sampler2D tex1;

void main()  {
	color =  texture(tex1,UV);
}