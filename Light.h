#pragma once
#include "OpenGL_Common.h"

class PointLight {
public:
	glm::vec3 color;
	glm::vec3 position;
};

class DirectionLight {
public:
	glm::vec3 color;
	glm::vec3 direction;
};

class SpotLight {
public:
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 front;
	float inner_cos;
	float outter_cos;
};
