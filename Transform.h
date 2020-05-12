#pragma once
#include "OpenGL_Common.h"

class Transform
{
public:
	glm::vec3 scale;
	//glm::vec3 rotation;
	glm::vec3 translation;

	Transform() :scale(glm::vec3(1)), translation(glm::vec3(0))
	{}

	Transform(glm::vec3 scale, glm::vec3 translation = glm::vec3(0)) :scale(scale), translation(translation)
	{}

	glm::mat4 getTransform()
	{
		glm::mat4 model;
		model = glm::translate(model, translation);
		model = glm::scale(model, scale);
		return model;
	}
};