#pragma once
#include <glad/glad.h>

#include "RenderObject.h"
#include "PostProcessor.h"

class Renderer
{
public:

    Renderer();

    virtual ~Renderer();

    virtual void Draw(RenderObject* renderObject) = 0;

    virtual void Draw(std::vector<RenderObject*>& renderObjects) = 0;

    void addPostProcessor(Shader shader, int width, int height);

protected:
    PostProcessor* postProcessor;
};

