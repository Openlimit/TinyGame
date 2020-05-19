#pragma once
#include "OpenGL_Common.h"

class Light
{
public:
	enum class LightType
	{
		DIRECTION,
		POINT,
		SPOT
	};
};

class PointLight:public Light
{
public:
	glm::vec3 color;
	glm::vec3 position;
};

class DirectionLight :public Light
{
public:
	glm::vec3 color;
	glm::vec3 direction;
};

class SpotLight :public Light
{
public:
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 front;
	float inner_cos;
	float outter_cos;
};
