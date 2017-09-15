#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.h"
#include "Cube.h"

void input_handler(GLFWwindow* window, Camera* camera, Cube* sky);
