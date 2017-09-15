#include "Cube.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Cube::Cube()
{
	_vertexVBO = 0;
	_uvVBO = 0;
	_textureID = 0;
	_programID = -1;
}

Cube::~Cube()
{
	if (_vertexVBO != 0){
		glDeleteBuffers(1, &_vertexVBO);
	}

	if (_uvVBO != 0){
		glDeleteBuffers(1, &_uvVBO);
	}
}

Transform& Cube::getTransform(){
	return _transform;
}


void Cube::init(){

	// vertices
	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	GLfloat uvCoords[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};

	_programID = loadShader("shaders/colorShading.vshader", "shaders/colorShading.fshader", _vShaderID, _fShaderID);

	// create VAO
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);
	
	if (_vertexVBO == 0){
		glGenBuffers(1, &_vertexVBO);
	}
	
	
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	// Give our vertices to OpenGL.

	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

	// buffer
	glGenBuffers(1, &_uvVBO);

	glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec2),
		uvCoords, GL_STATIC_DRAW);

	//loadTexture("textures/crate.png");
	loadCubeTexture();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);

}

void Cube::draw(glm::mat4 VP){

	glUseProgram(_programID);
	glBindVertexArray(_vaoID);

	// 1rst attribute buffer : vertices
	GLuint vertex_point_id = glGetAttribLocation(_programID, "vpoint");
	glEnableVertexAttribArray(vertex_point_id);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);

	glVertexAttribPointer(
		vertex_point_id,    // attribute 0
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalize?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);

	// tex attribute
	GLuint vertex_texture_coord_id = glGetAttribLocation(_programID,
		"vtexcoord");
	glEnableVertexAttribArray(vertex_texture_coord_id);
	glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	*/

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);

	//_rotateAngle += 0.01f;

	//_rotationMatrix = glm::rotate(_rotateAngle, glm::vec3(0.0f, 1.0f, 1.0f));
	glm::mat4 MVP = VP * _transform.getModelMatrix();

	GLint matLocation = glGetUniformLocation(_programID, "MVP");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	glDrawArrays(GL_TRIANGLES, 0, 36); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}



void Cube::loadCubeTexture(){

    // make stbi use same coordinates as opengl
    stbi_set_flip_vertically_on_load(1);

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
    int nb_component;

    std::vector<std::string> textures_faces;
    textures_faces.push_back("textures/miramar_ft.tga");
    textures_faces.push_back("textures/miramar_bk.tga");
    textures_faces.push_back("textures/miramar_dn.tga");
    textures_faces.push_back("textures/miramar_up.tga");
    textures_faces.push_back("textures/miramar_rt.tga"); // front  - rt
    textures_faces.push_back("textures/miramar_lf.tga"); // back - lf

    int width,height;
    unsigned char* image;
    for(GLuint i = 0; i < textures_faces.size(); i++)
    {

        image = stbi_load(textures_faces[i].c_str(), &width,
                                         &height, &nb_component, 0);

        if(image == nullptr) {
           printf("failed to load texture");
        }
        if(nb_component == 3) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);                
        } else if(nb_component == 4) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        }

        stbi_image_free(image);

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GLuint tex_id = glGetUniformLocation(_programID, "tex");
    glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cube::loadTexture(const char* filePath){
	// load texture
		{
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


			glGenTextures(1, &_textureID);
			glBindTexture(GL_TEXTURE_2D, _textureID);
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

			_textureID = glGetUniformLocation(_programID, "tex");
			glUniform1i(_textureID, 0 /*GL_TEXTURE0*/);

			// cleanup
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(image);
		}
}

GLuint Cube::loadShader(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID){

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
		printf("Can't open %s. Is path valid? !\n\n", vertex_file_path);
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