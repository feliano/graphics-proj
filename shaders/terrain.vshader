#version 330 core
#define M_PI 3.14f
layout(location = 0) in vec4 vpoint;
layout(location = 1) in vec2 vtexcoord;

uniform mat4 MVP;
uniform float time;
out vec2 UV;

uniform sampler2D tex2;

void main() {

	vec4 newCoords = MVP * vpoint;
	newCoords.y = newCoords.y + texture(tex2,vtexcoord).x;
	gl_Position = newCoords;
	UV = vtexcoord;
}
