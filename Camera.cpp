#include "Camera.h"

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
