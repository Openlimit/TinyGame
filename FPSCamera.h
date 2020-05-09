#pragma once
#include "OpenGL_Common.h"
#include "Camera.h"

class FPSCamera :public Camera
{
public:
    FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH) : Camera(position, up, yaw, pitch)
    {}

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
        if (Keys[GLFW_KEY_W])
            ProcessKeyboard(Camera_Movement::FORWARD, dt);
        if (Keys[GLFW_KEY_S])
            ProcessKeyboard(Camera_Movement::BACKWARD, dt);
        if (Keys[GLFW_KEY_A])
            ProcessKeyboard(Camera_Movement::LEFT, dt);
        if (Keys[GLFW_KEY_D])
            ProcessKeyboard(Camera_Movement::RIGHT, dt);
    }

    void ProcessMouse(double xpos, double ypos) override
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        ProcessMouseMovement(xoffset, yoffset);
    }

private:
    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == Camera_Movement::FORWARD)
            Position += Front * velocity;
        if (direction == Camera_Movement::BACKWARD)
            Position -= Front * velocity;
        if (direction == Camera_Movement::LEFT)
            Position -= Right * velocity;
        if (direction == Camera_Movement::RIGHT)
            Position += Right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }
};