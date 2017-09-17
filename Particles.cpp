#include "Particles.h"
#include "stb_image.h"


Particles::Particles(){

	_vertexVBO = 0;
	_uvVBO = 0;
	_textureID = 0;
	_programID = -1;

	particleContainer = new Particle[MAX_PARTICLES];

}

Particles::~Particles(){

	if (_vertexVBO != 0){
		glDeleteBuffers(1, &_vertexVBO);
	}

	if (_uvVBO != 0){
		glDeleteBuffers(1, &_uvVBO);
	}

	delete[] particleContainer;

}

Transform& Particles::getTransform(){
	return _transform;
}


void Particles::init(){

	GLfloat vertices[] = {
		-0.5f, 0.5f, 0.0f,  // upper left
		0.5f,  0.5f, 0.0f,  // upper right
		-0.5f, -0.5f, 0.0f, // lower left
		0.5f, -0.5f, 0.0f,  // lower right
	};

	GLfloat uvCoords[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	float particle_position_data[MAX_PARTICLES*4];

	for(int i = 0;i<MAX_PARTICLES;i++){

		particleContainer[i].pos.x = (((float) (rand()%(10000+1))) / float(10000)) * 20.0f; // size between 0.1 and 0.25;
		particleContainer[i].pos.y = (((float) (rand()%(6000+1))) / float(10000)) * 60.0f;;
		particleContainer[i].pos.z = (((float) (rand()%(10000+1))) / float(10000)) * 20.0f; // size between 0.1 and 0.25;
		particleContainer[i].size = ((float) (rand()%(2000-500 + 1) + 500)) / float(10000); // size between 0.1 and 0.25


		particle_position_data[i*4+0] = particleContainer[i].pos.x;
		particle_position_data[i*4+1] = particleContainer[i].pos.y;
		particle_position_data[i*4+2] = particleContainer[i].pos.z;
		particle_position_data[i*4+2] = particleContainer[i].size;	
	}

	_programID = loadShader("shaders/quad.vshader", "shaders/quad.fshader", _vShaderID, _fShaderID);

	// create VAO
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);
	
	if (_vertexVBO == 0){
		glGenBuffers(1, &_vertexVBO);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, _vertexVBO);
	// Give our vertices to OpenGL.

	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);


	// buffer
	glGenBuffers(1, &_uvVBO);

	glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2),
		uvCoords, GL_STATIC_DRAW);

	loadTexture("textures/snowflake.png");
	
	// particle data buffers
	glGenBuffers(1,&_particles_pos_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _particles_pos_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), particle_position_data, GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);

}

void Particles::draw(glm::mat4 VP){

	glUseProgram(_programID);
	glBindVertexArray(_vaoID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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


	// particles pos attribute
	GLuint p_pos_id = glGetAttribLocation(_programID,"particle_pos");
	glEnableVertexAttribArray(p_pos_id);
	glBindBuffer(GL_ARRAY_BUFFER, _particles_pos_buffer);

	float particle_position_data[MAX_PARTICLES*4];
	for(int i = 0;i<MAX_PARTICLES;i++){

		particleContainer[i].pos.x = particleContainer[i].pos.x;
		if(particleContainer[i].pos.y < 0.0f) particleContainer[i].pos.y = rand() % 50;
		particleContainer[i].pos.y = particleContainer[i].pos.y - 0.05f; // applying simple gravity
		particleContainer[i].pos.z = particleContainer[i].pos.z;


		particle_position_data[i*4+0] = particleContainer[i].pos.x;
		particle_position_data[i*4+1] = particleContainer[i].pos.y;
		particle_position_data[i*4+2] = particleContainer[i].pos.z;
		particle_position_data[i*4+3] = particleContainer[i].size;

	}
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), particle_position_data, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		p_pos_id,    		// attribute
		4,                  // size
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	
	/*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
	*/

	//_rotateAngle += 0.01f;

	//_rotationMatrix = glm::rotate(_rotateAngle, glm::vec3(0.0f, 1.0f, 1.0f));
    glm::mat4 M = _transform.getModelMatrix();

    GLint modelMatrixLocation = glGetUniformLocation(_programID,"M");
    glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,glm::value_ptr(M));
	GLint matLocation = glGetUniformLocation(_programID, "VP");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(VP));

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices, reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // particle position and size -> 1

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, MAX_PARTICLES);

	glDisable(GL_BLEND);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Particles::loadTexture(const char* filePath){
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

			_textureID = glGetUniformLocation(_programID, "tex1");
			glUniform1i(_textureID, 0 /*GL_TEXTURE0*/);

			// cleanup
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(image);
		}
}

GLuint Particles::loadShader(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID){

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