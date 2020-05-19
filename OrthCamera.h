#pragma once
#include "OpenGL_Common.h"
#include "BSphere.h"

class OrthCamera
{
public:
    float left, right, top, bottom, near, far;
    glm::vec3 Front, Position, Up;

	OrthCamera(float left, float right, float bottom, float top, float near, float far) :
		left(left), right(right), bottom(bottom), top(top), near(near), far(far)
    {}

    void setFrame(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    {
        Front = front;
        Position = position;
        Up = up;
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix()
    {
        return glm::ortho(left, right, bottom, top, near, far);
    }

    bool FrustumIntersection(BSphere bs)
    {
        auto view = glm::lookAt(Position, Position + Front, Up);
        bs.center = glm::vec3(view * glm::vec4(bs.center, 1));

        if (bs.center.x - bs.radius < right && bs.center.x + bs.radius > left &&
            bs.center.y - bs.radius < top && bs.center.y + bs.radius > bottom &&
            bs.center.z - bs.radius < -near && bs.center.z + bs.radius > -far)
            return true;
        else
            return false;
    }
};
