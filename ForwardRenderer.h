#pragma once
#include "Renderer.h"

class ForwardRenderer:public Renderer
{
public:

    ForwardRenderer();

    ~ForwardRenderer();

    void Draw(RenderObject* renderObject) override;

    void Draw(std::vector<RenderObject*>& renderObjects) override;
};

