#define TINYOBJLOADER_IMPLEMENTATION
#include "Object3D.h"
#include "stb_image.h"

Object3D::Object3D(const char * object_file_path)
{
	_object_file_path = object_file_path;
	_vertexVBO = 0;
	_uvVBO = 0;
	_normalsVBO = 0;
	_textureID = 0;
	_programID = -1;

}

Object3D::~Object3D()
{
	if (_vertexVBO != 0){
		glDeleteBuffers(1, &_vertexVBO);
	}

	if (_uvVBO != 0){
		glDeleteBuffers(1, &_uvVBO);
	}

	if(_elementBufferID != 0){
		glDeleteBuffers(1, &_elementBufferID);
	}

	if(_normalsVBO != 0){
		glDeleteBuffers(1,&_normalsVBO);
	}

}

Transform& Object3D::getTransform(){
	return _transform;
}

void Object3D::attachShaders(const char * vertex_file_path, const char * fragment_file_path){
	_vshader_file_path = vertex_file_path;
	_fshader_file_path = fragment_file_path;
}

void Object3D::addTexture(const char * texture_file_path){

	GLuint texture_id = loadTexture(texture_file_path);
            
    std::ostringstream tex_name;
    tex_name << "tex" << _texture_ids.size();

    // necessary to inform gl of the uniforms name
    GLuint tex_id = glGetUniformLocation(_programID, tex_name.str().c_str());
    glProgramUniform1i(_programID, tex_id, _texture_ids.size());

	_texture_ids.push_back(texture_id);
}


void Object3D::init(){

    
    std::vector<tinyobj::material_t> materials;
	std::string error;
	int flags = 1;
	bool err = tinyobj::LoadObj(_shapes, materials, error,_object_file_path);
	
	if(err == 0) std::cout << "error loading .obj file ... is file valid?" << std::endl;

	_programID = loadShader(_vshader_file_path, _fshader_file_path, _vShaderID, _fShaderID);

	// create VAO
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	// Generate and load buffers 

	// Vertices
	if (_vertexVBO == 0){
		glGenBuffers(1, &_vertexVBO);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, _shapes[0].mesh.positions.size() * sizeof(float), &_shapes[0].mesh.positions[0], GL_STATIC_DRAW);

	// UV 
	if(_uvVBO == 0){
		glGenBuffers(1, &_uvVBO);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
	glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(glm::vec3),
		&_uvs[0], GL_STATIC_DRAW);

	// Normals
	if (_normalsVBO == 0){
		glGenBuffers(1, &_normalsVBO);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, _shapes[0].mesh.normals.size() * sizeof(float), 
				&_shapes[0].mesh.normals[0], GL_STATIC_DRAW);

    // index buffer
    glGenBuffers(1, &_elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,_shapes[0].mesh.indices.size() * sizeof(unsigned int),
                &_shapes[0].mesh.indices[0], GL_STATIC_DRAW);


}

void Object3D::draw(glm::mat4 V,glm::mat4 P){

	glm::mat4 VP = P * V;

	glUseProgram(_programID);
	glBindVertexArray(_vaoID);

	GLuint vertex_point_id = glGetAttribLocation(_programID, "vpoint");
	glEnableVertexAttribArray(vertex_point_id);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);

	glVertexAttribPointer(
		vertex_point_id,    // attribute 0
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalize
		0,                  // stride
		(void*)0            // buffer offset
		);

	// THIS CAUSES SEGFAULT, TODO: DEBUG..
	/*
	glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
	
	// tex attribute
	GLuint vertex_texture_coord_id = glGetAttribLocation(_programID,
		"vtexcoord");
	glEnableVertexAttribArray(vertex_texture_coord_id);
	glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	
	*/

	// activate all loaded textures
	auto it = _texture_ids.begin();
	GLuint texture_count = 0;
	while(it != _texture_ids.end()){
		glActiveTexture(GL_TEXTURE0 + texture_count);
		glBindTexture(GL_TEXTURE_2D, *it);
		++texture_count;
		++it;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferID);

	// normals
	GLuint normals_id = glGetAttribLocation(_programID,
		"normal");
	glEnableVertexAttribArray(normals_id);
	glVertexAttribPointer(normals_id, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glm::mat4 MVP = VP * _transform.getModelMatrix();
	glm::mat4 MV = V * _transform.getModelMatrix();
	glm::mat4 normalMatrix = glm::inverseTranspose(MV);
	//glm::mat4 normalMatrix = glm::transpose(glm::inverse((glm::mat3(V * _transform.getModelMatrix()))));

	// passing the MVP to the shader program
	GLint matLocation = glGetUniformLocation(_programID, "MVP");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	// passing the MVP to the shader program
	GLint normalMatLocation = glGetUniformLocation(_programID, "normalMatrix");
	glUniformMatrix4fv(normalMatLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// passing the modelmatrix to the shader program
	GLint modelMatLocation = glGetUniformLocation(_programID, "M");
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(_transform.getModelMatrix()));

	GLint viewMatLocation = glGetUniformLocation(_programID, "V");
	glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, glm::value_ptr(V));


	// TODO: create new class for light
	light_rot += 0.01f;
	if(light_rot >= 360.0f) light_rot = 0.0f;

	GLint light = glGetUniformLocation(_programID, "light");
	glm::vec3 light_pos = glm::vec3(4.0f*cos(light_rot),3.0f,4.0f*sin(light_rot));
	glUniform3fv(light, 1 ,glm::value_ptr(light_pos));

    glDrawElements(GL_TRIANGLES, /*num vertices*/ _shapes[0].mesh.indices.size(),
            GL_UNSIGNED_INT,0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

GLuint Object3D::loadTexture(const char* filePath){
	
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

GLuint Object3D::loadShader(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID){

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