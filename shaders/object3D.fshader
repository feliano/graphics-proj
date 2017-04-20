#version 330 core

in vec2 UV;
in vec3 light_dir;
in vec3 vcolor;
in vec3 view_dir;
in vec3 fnormal;

out vec3 color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 normalMatrix;
uniform sampler2D tex;
uniform vec3 light;

void main()  {

	//color = vcolor;


    vec3 n = normalize(fnormal);
    vec3 l = normalize(light_dir);
    float lambert = dot(n,l);

 	color = vec3(1.0f,1.0f,1.0f) * lambert;
	/*
	float distance = length(light - pos_worldspace);

	vec3 n = normalize(vec3(0.0f,0.0f,1.0f));
	vec3 l = normalize(light_direction_cameraspace);

	float cos_theta = dot(n,l);


	//float cos_theta = dot( normal,surfaceToLight);

	color = vec3(1.0f,1.0f,1.0f) * cos_theta;// / (distance*distance);
	*/

	/*
	if(eye_direction_cameraspace.xyz == 0.0f){
		color = vec3(0.8,0.0,0.3);		
	}else{
		color = l;
	}
	*/

	//color = light;
	//color =  texture(tex, UV).rgb;
	
}