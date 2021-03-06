#pragma once
#include "Renderer.h"

class DeferredRenderer:public Renderer
{
public:

    DeferredRenderer(GLuint width, GLuint height);

    ~DeferredRenderer();

    void Init(Scene* scene, Camera* camera) override;

    void Draw(Scene* scene, Camera* camera) override;

private:
    Texture2D* gPosition;
    Texture2D* gNormal;
    Texture2D* gAlbedoSpec;
    GLuint rboDepth;
    GLuint G_Buffer;

    GLuint VAO, VBO;

    // Initialize quad for rendering postprocessing texture
    void initRenderData();
};

