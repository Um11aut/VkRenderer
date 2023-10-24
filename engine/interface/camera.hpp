#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

class Camera {
private:
	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 view;
};