#pragma once
#include "Renderer.h"

class DeferredRenderer:public Renderer
{
public:

    DeferredRenderer(GLuint width, GLuint height);

    ~DeferredRenderer();

    void Draw(RenderObject* renderObject) override;

    void Draw(std::vector<RenderObject*>& renderObjects) override;

private:
    Texture2D* gPosition;
    Texture2D* gNormal;
    Texture2D* gAlbedoSpec;
    GLuint rboDepth;
    GLuint G_Buffer;

    GLuint VAO, VBO;

    GLuint Width, Height;

    // Initialize quad for rendering postprocessing texture
    void initRenderData();
};

