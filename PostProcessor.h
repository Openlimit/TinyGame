#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture2D.h"
#include "Shader.h"


class PostProcessor
{
public:
    // State
    Shader* PostProcessingShader;
    Texture2D* colorTexture;
    GLuint Width, Height;

    // Constructor
    PostProcessor(Shader* shader, GLuint width, GLuint height);

    ~PostProcessor();

    // Prepares the postprocessor's framebuffer operations before rendering the game
    void BeginRender();
    // Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render();

    void Render(Texture2D* gPosition);

private:
    // Render state
    GLuint FBO; 
    GLuint VAO, VBO;
    // Initialize quad for rendering postprocessing texture
    void initRenderData();
};

