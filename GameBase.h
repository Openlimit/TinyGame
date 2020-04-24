#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GameBase {
public:
    GLboolean Keys[1024];

    virtual void Init() = 0;
    // GameLoop
    virtual void ProcessInput(GLfloat dt) = 0;
    virtual void ProcessMouse(double xpos, double ypos) = 0;
    virtual void ProcessScroll(double xoffset, double yoffset) = 0;
    virtual void Update(GLfloat dt) = 0;
    virtual void Render() = 0;
};