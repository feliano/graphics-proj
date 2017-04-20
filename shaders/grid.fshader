#version 330 core


in vec2 UV;
out vec3 color;

uniform sampler2D tex;

void main()  {
	//vec3 test = vec3(texture(tex,vec2(0.5f,0.5f)));
	vec3 newColor = vec3(texture(tex, UV));
	color = newColor;// + vec3(UV.x,UV.y,0.3f) + test;

}