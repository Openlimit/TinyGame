#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "Shader.h"


// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or 
// Shake boolean. 
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor
{
public:
    // State
    Shader PostProcessingShader;
    Texture2D colorTexture;
    Texture2D depthTexture;
    GLuint Width, Height;
    int samples;

    // Constructor
    PostProcessor(Shader shader, GLuint width, GLuint height);

    // Prepares the postprocessor's framebuffer operations before rendering the game
    void BeginRender();
    // Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render();
private:
    // Render state
    GLuint FBO; 
    GLuint VAO;
    // Initialize quad for rendering postprocessing texture
    void initRenderData();
};

