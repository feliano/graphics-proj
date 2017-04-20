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
#include <vector>


class Camera {

	int _window_width;
	int _window_height;

	glm::vec3 _cam_pos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 _cam_look = glm::vec3(0.0f, 0.0f, -1.0f); // looks towards a position (0.0f,0.0f,0.0f)
	glm::vec3 _cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 _viewMatrix = lookAt(_cam_pos, _cam_pos + _cam_look, _cam_up);

	float _fov = 45.0f;
	glm::mat4 _projectionMatrix = glm::perspective(_fov, (float)_window_width/(float)_window_height, 0.1f, 100.0f);

public:
	Camera(int window_width = 1024, int window_height = 768): _window_width(window_width),_window_height(window_height){}

	glm::mat4 getViewProjectionMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	float getFOV();
	void setFOV(float fov);

	glm::vec3 getDir();
	glm::vec3 getPos();
	glm::vec3 getUp();

	void setDir(glm::vec3 dir);
	void setPos(glm::vec3 pos);
	void setUp(glm::vec3 up);
};


glm::mat4 Camera::getViewProjectionMatrix(){
	return _projectionMatrix * _viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix(){
	return _projectionMatrix;
}

glm::mat4 Camera::getViewMatrix(){
	return _viewMatrix;
}


float Camera::getFOV(){
	return _fov;
}

void Camera::setFOV(float fov){
	_fov = fov;
	_projectionMatrix = glm::perspective(_fov, (float)_window_width/(float)_window_height, 0.1f, 15.0f);
}

glm::vec3 Camera::getDir(){
	return _cam_look;
}

glm::vec3 Camera::getPos(){
	return _cam_pos;
}

glm::vec3 Camera::getUp(){
	return _cam_up;
}

void Camera::setDir(glm::vec3 dir){
	dir = normalize(dir);
	if(dir.y > 0.99f) dir.y = 0.99f;
	if(dir.y < -0.9f) { // prevent flickering due to axes aligning
		dir.y = -0.9f;
	}
	
	_cam_look = dir;

	_viewMatrix = lookAt(_cam_pos, _cam_pos + _cam_look, _cam_up);
}

void Camera::setPos(glm::vec3 pos){
	_cam_pos  = pos;
	_viewMatrix = lookAt(_cam_pos, _cam_pos + _cam_look, _cam_up);
}

void Camera::setUp(glm::vec3 up){
	_cam_up = normalize(up);
	_viewMatrix = lookAt(_cam_pos, _cam_pos + _cam_look, _cam_up);
}
