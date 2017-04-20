#version 330 core
#define M_PI 3.14f
layout(location = 0) in vec4 vpoint;
layout(location = 1) in vec2 vtexcoord;

uniform mat4 MVP;
uniform float time;
out vec2 UV;

void main() {

	vec4 newCoords = MVP * vpoint;
	
	gl_Position = newCoords;
	gl_Position.y = newCoords.y + 0.05f * sin((newCoords.x + time) * M_PI * 2.0f) * sin((newCoords.z + time) * M_PI * 2.0f);

	UV = vtexcoord;
}
