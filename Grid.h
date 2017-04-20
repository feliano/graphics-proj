#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Transform.h"

class Grid
{
public:
	Grid();
	~Grid();

	void init(int dim = 512);
	void draw(glm::mat4 vpMatrix,float time);
	void attachShaders(const char * vertex_file_path, const char * fragment_file_path);
	void addTexture(const char * texture_file_path);
	GLuint loadTexture(const char* filePath);
	void bindHeightMap(GLuint height_texture);
	Transform& getTransform();

private:

	void addVertex(float x, float y, std::vector<GLfloat>* vertices);
	void addIndex(int index, std::vector<GLuint>* indices);

	GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID);
	GLuint _vaoID;
	GLuint _vertexVBO;
	GLuint _uvVBO;

	Transform _transform;

	const char * _vshader_file_path = "shaders/grid.vshader";
	const char * _fshader_file_path = "shaders/grid.fshader";
	const char * _texture_file_path = "textures/crate.png";
	
	std::vector<GLuint> _texture_ids;
	
	GLuint _vShaderID;
	GLuint _fShaderID;
	GLuint _programID;
	GLuint _elementBufferID;

	std::vector<GLfloat> _vertices;
	std::vector<GLfloat> _UV;
	std::vector<GLuint> _indices;
	GLuint _textureID;

	
};

