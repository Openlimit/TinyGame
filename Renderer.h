#pragma once
#include <glad/glad.h>

#include "Scene.h"
#include "PostProcessor.h"
#include "ShadowProcessor.h"

class Renderer
{
public:
    enum class RendererType
    {
        FORWARD,
        DEFFERED
    };

    Renderer();

    virtual ~Renderer();

    virtual void Draw(Scene *scene) = 0;

    void addPostProcessor(Shader* shader, int width, int height);

    void addShadowProcessor(int width, int height);

protected:
    PostProcessor* postProcessor;
    ShadowProcessor* shadowProcessor;
};

