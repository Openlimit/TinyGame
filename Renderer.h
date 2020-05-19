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

    Renderer(GLuint width, GLuint height);

    virtual ~Renderer();

    virtual void Init(Scene* scene, Camera* camera) = 0;

    virtual void Draw(Scene *scene, Camera* camera) = 0;

    void addPostProcessor(Shader* shader, int width, int height);

    void addShadowProcessor(int width, int height);

    bool enablePointLightShadow;

protected:
    GLuint Width, Height;
    PostProcessor* postProcessor;
    ShadowProcessor* shadowProcessor;

    void Culling(Scene* scene, Camera* camera);
};

