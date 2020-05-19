#pragma once
#include <vector>
#include "OpenGL_Common.h"
#include "BSphere.h"

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    enum class Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float near_plane;
    float far_plane;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        near_plane = 0.1;
        far_plane = 100;
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        near_plane = 0.1;
        far_plane = 100;
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix(float wh_ratio)
    {
        return glm::perspective(glm::radians(Zoom), wh_ratio, near_plane, far_plane);
    }

    bool FrustumIntersection(BSphere bs, float wh_ratio)
    {
        auto view = glm::lookAt(Position, Position + Front, Up);
        bs.center = glm::vec3(view * glm::vec4(bs.center, 1));

        float half_h = near_plane * std::tanf(Zoom / 2);
        float half_w = wh_ratio * half_h;
        glm::vec4 planes[] =
        {
            glm::vec4(glm::normalize(glm::vec3(0.f, -near_plane, -half_h)), 0.f),//Top
            glm::vec4(glm::normalize(glm::vec3(0.f, near_plane, -half_h)), 0.f),//Bottom
            glm::vec4(glm::normalize(glm::vec3(near_plane, 0, -half_w)), 0.f),//Left
            glm::vec4(glm::normalize(glm::vec3(-near_plane, 0, -half_h)), 0.f),//Right
            glm::vec4(0, 0, -1, -near_plane),//Near
            glm::vec4(0, 0, 1, far_plane)//Far
        };

        for (int i = 0; i < 6; i++)
        {
            float distance = glm::dot(glm::vec4(bs.center, 1), planes[i]);
            if (distance < -bs.radius)
                return false;
        }
        return true;
    }

    virtual void ProcessScroll(double xoffset, float yoffset) = 0;

    virtual void ProcessInput(GLboolean* Keys, GLfloat dt) = 0;

    virtual void ProcessMouse(double xpos, double ypos) = 0;

};

