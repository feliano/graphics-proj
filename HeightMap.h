#pragma once
//#include "icg_helper.h"
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


static const int ONE = 1;
static const bool DONT_NORMALIZE = false;
static const bool DONT_TRANSPOSE = false;
static const int ZERO_STRIDE = 0;
static const void* ZERO_BUFFER_OFFSET = 0;
static const glm::mat4 IDENTITY_MATRIX = glm::mat4(1.0f);

// setup 1D color texture
#define NB_COLOR 2
GLfloat tex[NB_COLOR] = {0.0, 2.0 * 3.14};

// Permutation array
#define NB_PSEUDO_RAND_INTS 512 
GLint p[512] = {151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};


struct PASS {
        enum ENUM {
            FIRST=1, SECOND=2
        };
};

class HeightMap {

    private:
        GLuint _vShaderID, _fShaderID;
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_1_id_;           // texture ID 1
        GLuint texture_1d_id_;          // texture ID
        GLuint texture_perm_id_;


        float std_dev = 2;
        float num_of_tiles = 4.0;
        float amplitude = 1.0;
        int octaves = 3;
        float gain = 1.0;
        float offset = 1.0;
        int fractal_algo = 0; // 0 = fBm, 1=

        int efficient_gaussian_;

        int screenquad_width_;
        int screenquad_height_;

    public:

    GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path, GLuint& vShaderID, GLuint& fShaderID){

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
    GLuint _programID = glCreateProgram();
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

        void Init(int width, int height, GLuint texture) {

            // set screenquad size
            this->screenquad_width_ = width;
            this->screenquad_height_ = height;

            // compile the shaders
            //program_id_ = icg_helper::LoadShaders("height_vshader.glsl",
            //                                      "height_fshader.glsl");
            program_id_ = loadShaders("shaders/height.vshader","shaders/heightmap.fshader",_vShaderID,_fShaderID);

            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex Array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates
            {
                const GLfloat vertex_point[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                                                 /*V2*/ +1.0f, -1.0f, 0.0f,
                                                 /*V3*/ -1.0f, +1.0f, 0.0f,
                                                 /*V4*/ +1.0f, +1.0f, 0.0f
            };
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                             vertex_point, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_point_id = glGetAttribLocation(program_id_, "vpoint");
                glEnableVertexAttribArray(vertex_point_id);
                glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // texture coordinates
            {
                const GLfloat vertex_texture_coordinates[] = { /*V1*/ 0.0f, 0.0f,
                                                               /*V2*/ 1.0f, 0.0f,
                                                               /*V3*/ 0.0f, 1.0f,
                                                               /*V4*/ 1.0f, 1.0f

            };

                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_coordinates),
                             vertex_texture_coordinates, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_texture_coord_id = glGetAttribLocation(program_id_,
                                                                     "vtexcoord");
                glEnableVertexAttribArray(vertex_texture_coord_id);
                glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT,
                                      DONT_NORMALIZE, ZERO_STRIDE,
                                      ZERO_BUFFER_OFFSET);
            }

            // load/Assign texture
            //this->texture_1_id_ = texture;
            //glBindTexture(GL_TEXTURE_2D, texture_1_id_);
            //float * data = new float[screenquad_width_*screenquad_height_];
            //glTexImage2D(GL_TEXTURE_2D,0,GL_R16F,screenquad_width_,screenquad_height_,0,GL_RED,GL_FLOAT,data);
        
            
            // 1D texture for gradients shader
            glGenTextures(1, &texture_1d_id_);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_1D, texture_1d_id_);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, NB_COLOR, 0, GL_RED, GL_FLOAT, tex);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glBindTexture(GL_TEXTURE_1D, texture_1d_id_);
            
            glUniform1i(glGetUniformLocation(program_id_, "tex_PASS1"),
                        0 /*GL_TEXTURE0*/);


            // 1D texture for permutation table
            
            glGenTextures(1, &texture_perm_id_);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_1D, texture_perm_id_);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, NB_PSEUDO_RAND_INTS, 0, GL_RED, GL_UNSIGNED_BYTE, p);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glBindTexture(GL_TEXTURE_1D, texture_perm_id_);
            
            glUniform1i(glGetUniformLocation(program_id_, "perm_table"),
                        1 /*GL_TEXTURE1*/);
            



            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteTextures(1, &texture_1_id_);
        }

        void UpdateSize(int screenquad_width, int screenquad_height) {
            this->screenquad_width_ = screenquad_width;
            this->screenquad_height_ = screenquad_height;
        }


        void UpdateNumberOfTiles(float tiles){
            this->num_of_tiles += tiles;
            std::cout << "new num of tiles : " << num_of_tiles << std::endl;
        }
        
        void UpdateFractalAmplitude(float increment){
            this->amplitude += increment;
            std::cout << "new amplitude : " << amplitude << std::endl;
        }

        void UpdateNumberOfOctaves(int increment){
            this->octaves += increment;
            std::cout << "new num of octaves : " << octaves << std::endl;
        }

        void UpdateGain(float increment){
            this->gain += increment;
            std::cout << "new gain : " << gain << std::endl;
        }

        void UpdateOffset(float increment){
            this->offset += increment;
            std::cout << "new offset : " << offset << std::endl;
        }

        void UseFractalAlgorithm(int value){
            this->fractal_algo = value;
            if( value == 0){
                std::cout << "using fBm : " << offset << std::endl;
            }else if( value == 1){
                std::cout << "using Ridged Multifractal : " << offset << std::endl;
            }else{
                std::cout << "using hybrid Multifractal : " << offset << std::endl;
            }

        }

        void Draw() {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);
            glUniform1f(glGetUniformLocation(program_id_, "num_tiles"), num_of_tiles);
            glUniform1f(glGetUniformLocation(program_id_, "H"), amplitude);
            glUniform1i(glGetUniformLocation(program_id_, "octaves"), octaves);
            glUniform1f(glGetUniformLocation(program_id_, "gain"), gain);
            glUniform1f(glGetUniformLocation(program_id_, "offset"), offset);
            glUniform1i(glGetUniformLocation(program_id_, "fractal_algorithm"), fractal_algo);
            glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_1D, texture_1_id_);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
