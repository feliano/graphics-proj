#version 330 core
layout(location = 0) in vec4 vpoint;
layout(location = 1) in vec2 vtexcoord;

in vec3 normal;

out vec2 UV;
out vec3 light_dir;
out vec3 vcolor;
out vec3 fnormal;
out vec3 view_dir;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 normalMatrix;
uniform vec3 light;

void main() {


    vec4 vpoint_m = M * vpoint;
    gl_Position = MVP * vpoint;
    vcolor = vec3(0.0,0.0,0.0);
    //vec3 normal_mv = mat3(normalMatrix) * normal;
    vec3 normal_mv = mat3(transpose(inverse(M))) * normal;

    light_dir = light - vpoint_m.xyz;
    view_dir = -vpoint_m.xyz;
    
   	fnormal = normal_mv;
    vec3 n = normalize(normal);
    vec3 l = normalize(light_dir);
    float lambert = dot(n,l);

    vcolor = vec3(0.8f,0.3,0.4) * lambert;



	// old
	/*
    vec4 vpoint_mv = V * M * vpoint;
    gl_Position = MVP * vpoint;
    vcolor = vec3(0.0,0.0,0.0);
    vec3 normal_mv = mat3(normalMatrix) * normal;
    //vec3 normal_mv = mat3(transpose(inverse(V*M))) * normal;

    light_dir = light - vpoint_mv.xyz;
    view_dir = -vpoint_mv.xyz;
    
   	fnormal = normal_mv;
    vec3 n = normalize(normal);
    vec3 l = normalize(light_dir);
    float lambert = dot(n,l);

    vcolor = vec3(0.8f,0.3,0.4) * lambert;
	*/

	// OLD
	/*
	vec4 newPosition = MVP * vpoint;
	gl_Position = newPosition;

	pos_worldspace = (M * vpoint).xyz;

	vec3 vertexPos_cameraspace = (V * M * vpoint).xyz;
	//vec4 test = vec4(1.0f,1.0f,1.0f,1.0f);
	eye_direction_cameraspace = vec3(0.0f,0.0f,0.0f) - vertexPos_cameraspace;

	vec3 light_pos_cameraspace = ( V * vec4(light,1)).xyz;
	light_direction_cameraspace = light_pos_cameraspace + eye_direction_cameraspace;

	vnormal = (normalMatrix * vec4(normal,1)).xyz;
	*/


	//light_dir = light;
	UV = vtexcoord;
}
