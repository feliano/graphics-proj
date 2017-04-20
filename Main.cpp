#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

#include "Camera.h"
// Include 3d Objects
#include "Cube.h"
#include "Grid.h"
#include "Terrain.h"
#include "Skybox.h"
#include "HeightMap.h"
#include "Object3D.h"

#include "FrameBuffer.h"

// Include common methods for loading shaders
#include <common/shader.hpp>

GLFWwindow* window;
Camera camera;
Terrain terrain;
Grid quad;
Skybox sky;
Object3D obj("objects/sphere.obj");
FrameBuffer frameBuffer;
HeightMap heightMap;
GLuint fb_tex;
/*
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
*/

void input_handler(){

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		glm::vec3 axis = glm::cross(camera.getDir(),camera.getUp());
		glm::quat myQuaternion;
		myQuaternion = glm::angleAxis(glm::radians(1.0f),axis);
		glm::mat4 rotation = glm::mat4_cast(myQuaternion);
		camera.setDir(glm::mat3(rotation) * camera.getDir());

	}else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		glm::vec3 axis = glm::cross(camera.getDir(),camera.getUp());
		glm::mat4 rotate = glm::rotate(glm::radians(-1.0f), axis);
		camera.setDir(glm::mat3(rotate) * camera.getDir());
	}else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		glm::mat4 rotate = glm::rotate(glm::radians(1.0f), glm::vec3(0.0f,1.0f,0.0f));
		camera.setDir(glm::mat3(rotate) * camera.getDir());
	}else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		glm::mat4 rotate = glm::rotate(glm::radians(-1.0f), glm::vec3(0.0f,1.0f,0.0f));
		camera.setDir(glm::mat3(rotate) * camera.getDir());
	}else if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		camera.setPos(camera.getPos()-0.1f*camera.getDir());
		// move skybox along with camera
		sky.getTransform().translate(-0.1f*camera.getDir());

	}else if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
		camera.setPos(camera.getPos()+0.1f*camera.getDir());
		// move skybox along with camera
		sky.getTransform().translate(0.1f*camera.getDir());

	}else if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
		camera.setFOV(camera.getFOV() + 0.01f);
	}else if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
		camera.setFOV(camera.getFOV() - 0.01f);
	}else if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
		// Turn on wireframe rendering
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}else if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
		// Turn off wireframe rendering
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}

	if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
		heightMap.UpdateNumberOfOctaves(1);
	}else if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		heightMap.UpdateNumberOfOctaves(-1);
	}


	if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
		heightMap.UpdateNumberOfTiles(0.1f);
	}else if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
		heightMap.UpdateNumberOfTiles(-0.1f);
	}

	if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
		heightMap.UpdateFractalAmplitude(0.1f);
	}else if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
		heightMap.UpdateFractalAmplitude(-0.1f);
	}

	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
		heightMap.UseFractalAlgorithm(0);
	}else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
		heightMap.UseFractalAlgorithm(1);
	}else if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
		heightMap.UseFractalAlgorithm(2);
	}

	if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
		heightMap.UpdateGain(0.1f);
	}else if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS){
		heightMap.UpdateGain(-0.1f);
	}

	if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS){
		heightMap.UpdateOffset(0.1f);
	}else if(glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS){
		heightMap.UpdateOffset(-0.1f);
	}

}

void init(){
		// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open window and create OpenGL context
	window = glfwCreateWindow( 1024, 768, "Graphics", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window\n" );
		getchar();
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; // magic needed to get things working 

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
	}

	// activate sticky keys
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// enable depth test
	glEnable(GL_DEPTH_TEST);
	// accept fragment closest to camera
	glDepthFunc(GL_LESS);

    //glfwSetKeyCallback(window, KeyCallback);
	// gray background
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

	// OBJECTS
	quad.attachShaders("shaders/terrain.vshader","shaders/terrain.fshader"); // has to be done before init atm
	quad.init(1);
	sky.init();
	terrain.init();

	fb_tex = frameBuffer.Init(1024,768); // actually should be image_width & size
	GLuint temp; 
	heightMap.Init(1024,768,temp);

	frameBuffer.Bind();
	heightMap.Draw();
	//quad.draw(camera.getViewProjectionMatrix(),glfwGetTime());
	//sky.draw(camera.getViewProjectionMatrix());
	//terrain.draw(camera.getViewProjectionMatrix(),glfwGetTime());
	frameBuffer.Unbind();
	
	terrain.addTexture("textures/crate.png");
	terrain.addTexture("textures/ground.png");
	terrain.bindHeightMap(fb_tex);
	quad.bindHeightMap(fb_tex);

	// model matrix

	terrain.getTransform().rotate(90.0f,glm::vec3(1.0f,0.0f,0.0f));
	terrain.getTransform().scale(20.0f,20.0f,20.0f);

	sky.getTransform().scale(20.0f,20.0f,20.0f);
	sky.getTransform().rotate(180.0f,glm::vec3(1.0f,0.0f,0.0f));

	quad.getTransform().rotate(0.0f,glm::vec3(1.0f,0.0f,0.0f));


	obj.init();
	obj.getTransform().translate(glm::vec3(0.0f,2.0f,0.0f));

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	// check that fov is in range and update accordingly
	if(camera.getFOV() >= 1.0f && camera.getFOV() <= 60.0f){
		camera.setFOV(camera.getFOV() - yoffset*0.1f); 
		//projectionMatrix = glm::perspective(fov, (float)1280 / (float)720, 1.0f, 10.0f);
	}
	if(camera.getFOV() > 60.0f) camera.setFOV(60.0f);
	if(camera.getFOV() < 1.0f) camera.setFOV(1.0f);
}


int main( void )
{
	// init stuff
	init();

	GLint test;
	GLint vecs;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,&test);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS,&vecs);

	std::cout << "uniform comps: " << test << std::endl;
	std::cout << "uniform vecs: " << vecs << std::endl;

    // set the mouse press and position callback
    //glfwSetMouseButtonCallback(window, MouseButton);
    //glfwSetCursorPosCallback(window, MousePos);

	glfwSetScrollCallback(window, scroll_callback);

	do{

		// Clear the screen.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glMatrixMode (GL_PROJECTION);
		input_handler();

		// update heightmap
		frameBuffer.Bind();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // need to clear the viewport
		heightMap.Draw(); // rendering to framebuffers texture
		frameBuffer.Unbind();

		//std::cout << "time: " << glfwGetTime() << std::endl;
		// Draw all the objects
		//quad.draw(glm::mat4(1.0f),glfwGetTime()); // Debug visualize heightmap
		sky.draw(camera.getViewProjectionMatrix());
		terrain.draw(camera.getViewProjectionMatrix(),glfwGetTime());

		//obj.getTransform().rotate(1.0f,glm::vec3(0.0f,1.0f,0.0f));
		obj.draw(camera.getViewMatrix(),camera.getProjectionMatrix());
		//heightMap.Draw();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} 
	// Check if ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

