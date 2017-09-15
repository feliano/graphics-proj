#include "Controls.h"

void input_handler(GLFWwindow* window, Camera* camera, Cube* sky){

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		glm::vec3 axis = glm::cross(camera->getDir(),camera->getUp());
		glm::quat myQuaternion;
		myQuaternion = glm::angleAxis(glm::radians(1.0f),axis);
		glm::mat4 rotation = glm::mat4_cast(myQuaternion);
		camera->setDir(glm::mat3(rotation) * camera->getDir());
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		glm::vec3 axis = glm::cross(camera->getDir(),camera->getUp());
		glm::mat4 rotate = glm::rotate(glm::radians(-1.0f), axis);
		camera->setDir(glm::mat3(rotate) * camera->getDir());
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		glm::mat4 rotate = glm::rotate(glm::radians(1.0f), glm::vec3(0.0f,1.0f,0.0f));
		camera->setDir(glm::mat3(rotate) * camera->getDir());
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		glm::mat4 rotate = glm::rotate(glm::radians(-1.0f), glm::vec3(0.0f,1.0f,0.0f));
		camera->setDir(glm::mat3(rotate) * camera->getDir());
	}

	if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		camera->setPos(camera->getPos()-0.1f*camera->getDir());
		// move skybox along with camera	
		if(sky != nullptr) sky->getTransform().translate(-0.1f*camera->getDir());

	}

	if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
		camera->setPos(camera->getPos()+0.1f*camera->getDir());
		// move skybox along with camera		
		if(sky != nullptr) sky->getTransform().translate(0.1f*camera->getDir());

	}

	if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
		camera->setFOV(camera->getFOV() + 0.01f);
	}

	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
		camera->setFOV(camera->getFOV() - 0.01f);
	}

	if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
		// Turn on wireframe rendering
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}

	if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
		// Turn off wireframe rendering
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}


}