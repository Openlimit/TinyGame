#pragma once
#include <glad/glad.h>

#include "RenderObject.h"
#include "PostProcessor.h"

class Renderer
{
public:
    
    Renderer();
    
    ~Renderer();
    
    void Draw(RenderObject* renderObject);

    void Draw(std::vector<RenderObject*>& renderObjects);

    void addPostProcessor(Shader shader, int width, int height);

private:
    PostProcessor* postProcessor;
};

