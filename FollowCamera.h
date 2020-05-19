#pragma once
#include "OpenGL_Common.h"
#include "Camera.h"

class FollowCamera :public Camera
{
public:
    FollowCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 lookat = glm::vec3(0.0f, 0.0f, -1.0f)) : Camera(position)
    {
        Front = glm::normalize(lookat - position);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

    void ProcessScroll(double xoffset, float yoffset) override
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

    void ProcessInput(GLboolean* Keys, GLfloat dt) override
    {
        float velocity = MovementSpeed * dt;
        if (Keys[GLFW_KEY_W])
            Position += glm::vec3(0, 0, -1) * velocity;
        if (Keys[GLFW_KEY_S])
            Position += glm::vec3(0, 0, 1) * velocity;
        if (Keys[GLFW_KEY_A])
            Position += glm::vec3(-1, 0, 0) * velocity;
        if (Keys[GLFW_KEY_D])
            Position += glm::vec3(1, 0, 0) * velocity;
    }

    void ProcessMouse(double xpos, double ypos) override
    {
        
    }
};