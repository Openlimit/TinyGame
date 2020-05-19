#pragma once
#include "Renderer.h"

class ForwardRenderer:public Renderer
{
public:

    ForwardRenderer(GLuint width, GLuint height);

    ~ForwardRenderer();

    void Init(Scene* scene, Camera* camera) override;

    void Draw(Scene* scene, Camera* camera) override;
};

