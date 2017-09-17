#version 330 core
layout(location = 0) in vec4 vpoint;

in vec4 particle_pos;
in vec2 vtexcoord;

out vec2 UV;

uniform mat4 M;
uniform mat4 VP;

void main() {

	vec4 coords = vpoint;
	coords.x = coords.x*particle_pos.w + particle_pos.x;
	coords.y = coords.y*particle_pos.w + particle_pos.y;
	coords.z = coords.z*particle_pos.w + particle_pos.z;

	UV = vtexcoord;

	vec4 newPosition = VP * M * coords;
	gl_Position = newPosition;

}
