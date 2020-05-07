#pragma once
#include "Renderer.h"

class ForwardRenderer:public Renderer
{
public:

    ForwardRenderer();

    ~ForwardRenderer();

    void Draw(Scene* scene) override;
};

