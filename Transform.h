#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
//using namespace glm;

class Transform {
	
private:		
	glm::mat4 _modelMatrix = glm::mat4(1.0f);
	glm::mat4 _translationMatrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 _rotationMatrix = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 _scaleMatrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

public:
	void rotate(float angle, glm::vec3 axis){
		_rotationMatrix = _rotationMatrix * glm::rotate(glm::radians(angle),axis);
	}
	void scale(float x, float y, float z){
		_scaleMatrix = _scaleMatrix * glm::scale(glm::vec3(x,y,z));
	}

	void translate(glm::vec3 move_vec){
		_translationMatrix = _translationMatrix * glm::translate(move_vec);
	}

	glm::mat4 getModelMatrix(){
		_modelMatrix = _translationMatrix *_rotationMatrix * _scaleMatrix;
		return _modelMatrix;
	}


};
