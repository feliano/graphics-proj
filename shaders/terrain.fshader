#version 330 core


in vec2 UV;
out vec3 color;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main()  {
	//vec3 test = vec3(texture(tex,vec2(0.5f,0.5f)));
	vec3 newColor = vec3(texture(tex0, UV));
  vec3 extraColor = vec3(texture(tex1, UV));
  vec3 height = vec3(texture(tex2, UV));

	color = extraColor;// + vec3(UV.x,UV.y,0.3f) + test;
  //color = height;

}
