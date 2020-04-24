#pragma once
#include <vector>
#include "GameBase.h"
#include "Camera.h"
#include "Renderer.h"
#include "ResourceManager.h"

class WaterGame :public GameBase
{
public:
    GLuint Width, Height;
    Camera camera;
    Renderer renderer;

    Mesh* plane;
    Shader waterShader;

    float time;

    WaterGame(GLuint width, GLuint height);
    ~WaterGame();

    void Init() override;
    void ProcessInput(GLfloat dt) override;
    void ProcessMouse(double xpos, double ypos) override;
    void ProcessScroll(double xoffset, double yoffset) override;
    void Update(GLfloat dt) override;
    void Render() override;
};

