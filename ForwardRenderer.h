#pragma once
#include "Renderer.h"

class ForwardRenderer:public Renderer
{
public:

    ForwardRenderer(GLuint width, GLuint height);

    ~ForwardRenderer();

    void Init(Scene* scene) override;

    void Draw(Scene* scene) override;
};

