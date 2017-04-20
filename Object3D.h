#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Transform.h"
#include "common/tiny_obj_loader.h"

class Object3D
{
public:
	Object3D(const char * object_file_path);
	~Object3D();

	void init(); // parse obj and set up buffers.
	void draw(glm::mat4 V,glm::mat4 P);
	void attachShaders(const char * vertex_file_path, const char * fragment_file_path);
	void addTexture(const char * texture_file_path);
	GLuint loadTexture(const char* filePath);
	Transform& getTransform();

private:

	GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID);
	GLuint _vaoID;
	GLuint _vertexVBO;
	GLuint _uvVBO;
	GLuint _normalsVBO;

	Transform _transform;

	const char * _vshader_file_path = "shaders/object3D.vshader";
	const char * _fshader_file_path = "shaders/object3D.fshader";
	const char * _texture_file_path = "textures/crate.png";
	const char * _object_file_path = "object/cube.obj";


	std::vector<tinyobj::shape_t> _shapes;
	

	std::vector<GLuint> _texture_ids;
	
	GLuint _vShaderID;
	GLuint _fShaderID;
	GLuint _programID;
	GLuint _elementBufferID;

	float light_rot = 0.0f;

	std::vector< glm::vec3 > _vertices;
	std::vector< glm::vec2 > _uvs;
	std::vector< glm::vec3 > _normals;
	GLuint _textureID;

	
};

