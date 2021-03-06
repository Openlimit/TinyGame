#pragma once
#include <vector>
#include "GameBase.h"

class WaterGame :public GameBase
{
public:
    Shader* postShader;
    float time;

    WaterGame(GLuint width, GLuint height);
    ~WaterGame();

    void Init() override;
    void ProcessInput(GLfloat dt) override;
    void ProcessMouse(double xpos, double ypos) override;
    void ProcessScroll(double xoffset, double yoffset) override;
    void Update(GLfloat dt) override;
};

