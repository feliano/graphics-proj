#include "Grid.h"
#include "stb_image.h"



Grid::Grid()
{
	_vertexVBO = 0;
	_uvVBO = 0;
	_textureID = 0;
	_programID = -1;	
	_elementBufferID = 0;
}

Grid::~Grid()
{
	if (_vertexVBO != 0){
		glDeleteBuffers(1, &_vertexVBO);
	}

	if (_uvVBO != 0){
		glDeleteBuffers(1, &_uvVBO);
	}

	if (_elementBufferID != 0){
		glDeleteBuffers(1, &_elementBufferID);
	}
}

void Grid::addVertex(float x, float y, std::vector<GLfloat>* vertices){
	(*vertices).push_back((x-0.5f)*2.0f);
	(*vertices).push_back((y-0.5f)*2.0f);
	_UV.push_back(x);
	_UV.push_back(y);
}


void Grid::addIndex(int index, std::vector<GLuint>* indices){
	(*indices).push_back(index);
}

Transform& Grid::getTransform(){
	return _transform;
}


void Grid::attachShaders(const char * vertex_file_path, const char * fragment_file_path){
	_vshader_file_path = vertex_file_path;
	_fshader_file_path = fragment_file_path;
}

void Grid::addTexture(const char * texture_file_path){

	GLuint texture_id = loadTexture(texture_file_path);
            
    std::ostringstream tex_name;
    tex_name << "tex" << _texture_ids.size();

    // necessary to inform gl of the uniforms name
    GLuint tex_id = glGetUniformLocation(_programID, tex_name.str().c_str());
    glProgramUniform1i(_programID, tex_id, _texture_ids.size());

	_texture_ids.push_back(texture_id);
}

void Grid::bindHeightMap(GLuint height_texture){

	//GLuint texture_id = loadTexture(texture_file_path);
            
    std::ostringstream tex_name;
    tex_name << "tex" << _texture_ids.size();

    // necessary to inform gl of the uniforms name
    GLuint tex_id = glGetUniformLocation(_programID, tex_name.str().c_str());
    glProgramUniform1i(_programID, tex_id, _texture_ids.size());

	std::cout << tex_name.str() << std::endl;
    std::cout << height_texture << std::endl;


	_texture_ids.push_back(height_texture);
}



void Grid::init(int dim){

	//int dim = 512;
	// vertices
	float step = 1 / (float)dim;

	for (int i = 0; i < dim+1; i++){
		for (int j = 0; j < dim+1; j++){
			addVertex(j*step, i*step, &_vertices);
		}
	}

	// indices
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			addIndex(i*(dim + 1) + j, &_indices);
			addIndex(i*(dim + 1) + j + 1, &_indices);
			addIndex((i + 1)*(dim + 1) + j, &_indices);
			addIndex((i + 1)*(dim + 1) + j, &_indices);
			addIndex((i + 1)*(dim + 1) + j + 1, &_indices);
			addIndex(i*(dim + 1) + j + 1, &_indices);
		}
	}
	int count = 0;
	for (auto i = _indices.begin(); i != _indices.end(); ++i){
		count++;
	}

	_programID = loadShader(_vshader_file_path, _fshader_file_path, _vShaderID, _fShaderID);

	// create VAO
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);


	if (_vertexVBO == 0){
		glGenBuffers(1, &_vertexVBO);
	}

	// The following commands will talk about our 'vertexbuffer' buffer

	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	// Give our vertices to OpenGL.

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);

	// UV 
	glGenBuffers(1, &_uvVBO);

	glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
	glBufferData(GL_ARRAY_BUFFER, _UV.size() * sizeof(GLfloat),
		&_UV[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() *sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);

}

void Grid::draw(glm::mat4 VP,float time){

	glUseProgram(_programID);
	glBindVertexArray(_vaoID);

	// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(0);
	GLuint vertex_point_id = glGetAttribLocation(_programID, "vpoint");
	glEnableVertexAttribArray(vertex_point_id);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);

	glVertexAttribPointer(
		vertex_point_id,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);

	// tex attribute
	GLuint vertex_texture_coord_id = glGetAttribLocation(_programID,
		"vtexcoord");
	glEnableVertexAttribArray(vertex_texture_coord_id);
	glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);


	auto it = _texture_ids.begin();
	GLuint texture_count = 0;
	while(it != _texture_ids.end()){

		glActiveTexture(GL_TEXTURE0 + texture_count);
		glBindTexture(GL_TEXTURE_2D, *it);
		++texture_count;
		++it;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferID);

	glm::mat4 MVP = VP * _transform.getModelMatrix();

	// passing the MVP to the shader program
	GLint matLocation = glGetUniformLocation(_programID, "MVP");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	// other uniforms
	GLint time_loc = glGetUniformLocation(_programID,"time");
	glUniform1f(time_loc,time);

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

GLuint Grid::loadTexture(const char* filePath){
	// load texture
	
		GLuint texture_id;
		int width;
		int height;
		int nb_component;
		std::string filename = filePath;
		// set stb_image to have the same coordinates as OpenGL
		stbi_set_flip_vertically_on_load(1);
		unsigned char* image = stbi_load(filename.c_str(), &width,
			&height, &nb_component, 0);

		if (image == nullptr) {
			throw(std::string("Failed to load texture"));
		}

		glGenTextures(1, &texture_id);

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (nb_component == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image);
		}
		else if (nb_component == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, image);
		}

		// cleanup
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	

	return texture_id;

}

GLuint Grid::loadShader(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID){

	// Create the shaders
	vShaderID = glCreateShader(GL_VERTEX_SHADER);
	fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()){
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		printf("Can't open %s. Is path valid? !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(vShaderID);

	// Check Vertex Shader
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fShaderID);

	// Check Fragment Shader
	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	_programID = glCreateProgram();
	glAttachShader(_programID, vShaderID);
	glAttachShader(_programID, fShaderID);

	glLinkProgram(_programID);

	// Check the program
	glGetProgramiv(_programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(_programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(_programID, vShaderID);
	glDetachShader(_programID, fShaderID);

	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);

	return _programID;
}