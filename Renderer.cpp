#include "Renderer.h"

Renderer::Renderer(GLuint width, GLuint height) :Width(width), Height(height), enablePointLightShadow(true)
{
}

Renderer::~Renderer()
{
    if (this->postProcessor != nullptr)
        delete this->postProcessor;
    if (this->shadowProcessor != nullptr)
        delete this->shadowProcessor;
}

void Renderer::addPostProcessor(Shader* shader, int width, int height)
{
    this->postProcessor = new PostProcessor(shader, width, height);
}

void Renderer::addShadowProcessor(int width, int height)
{
    this->shadowProcessor = new ShadowProcessor(width, height);
}