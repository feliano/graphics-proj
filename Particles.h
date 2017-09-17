#pragma once
#include "Transform.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Particle{
	glm::vec3 pos,dir;
	float size = 0.5f;
};


class Particles{

public:
	Particles();
	~Particles();

	void init();
	void draw(glm::mat4 VP);
	Transform& getTransform();

private:

	GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID);
	void loadTexture(const char* filePath);
	
	// general for 3d objects
	GLuint _vaoID;
	GLuint _vertexVBO;
	GLuint _uvVBO;
	GLuint _particles_pos_buffer;

	GLuint _vShaderID;
	GLuint _fShaderID;
	GLuint _programID;

	GLuint _textureID;
	Transform _transform;

	// particles specific
	const int MAX_PARTICLES = 10000;
	Particle* particleContainer;

};

