#version 330 core
layout(location = 0) in vec4 vpoint;
layout(location = 1) in vec2 vtexcoord;

out vec2 UV;
out vec3 texCoords;

uniform mat4 MVP;

void main() {

	vec4 newPosition = MVP * vpoint;
	gl_Position = newPosition;
	texCoords = vpoint.xyz;
	UV = vtexcoord;
}
