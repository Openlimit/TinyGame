#pragma once
#include "OpenGL_Common.h"
#include "RenderObject.h"

class GameObject
{
public:
	enum class MoveDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	RenderObject* renderObject;
	float move_speed;
	glm::vec3 move_direction;

	GameObject(RenderObject* renderObject):renderObject(renderObject), move_speed(0), move_direction(glm::vec3(0)) {}

	void move(MoveDirection dir, float dt)
	{
		switch (dir)
		{
		case MoveDirection::FORWARD:
			move_direction = glm::vec3(0, 0, -1);
			break;
		case MoveDirection::BACKWARD:
			move_direction = glm::vec3(0, 0, 1);
			break;
		case MoveDirection::LEFT:
			move_direction = glm::vec3(-1, 0, 0);
			break;
		case MoveDirection::RIGHT:
			move_direction = glm::vec3(1, 0, 0);
			break;
		default:
			break;
		}

		renderObject->transform.translation += move_speed * move_direction * dt;
	}
};

