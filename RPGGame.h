#pragma once
#include "GameBase.h"
#include "Camera.h"

class RPGGame :public GameBase
{
public:

    float time;

    RPGGame(GLuint width, GLuint height);
    ~RPGGame();

    void Init() override;
    void ProcessInput(GLfloat dt) override;
    void ProcessMouse(double xpos, double ypos) override;
    void ProcessScroll(double xoffset, double yoffset) override;
    void Update(GLfloat dt) override;
};

