#include "Renderer.h"

Renderer::Renderer()
{}

Renderer::~Renderer()
{
    if (postProcessor != nullptr)
        delete postProcessor;
}

void Renderer::addPostProcessor(Shader shader, int width, int height)
{
    postProcessor = new PostProcessor(shader, width, height);
}