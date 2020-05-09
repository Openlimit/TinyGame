#include "ShadowProcessor.h"
#include <iostream>

ShadowProcessor::ShadowProcessor(GLuint width, GLuint height):Width(width),Height(height)
{
    // Initializeframebuffer object
    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    this->depthTexture = new Texture2D();
    this->depthTexture->Image_Format = this->depthTexture->Internal_Format = GL_DEPTH_COMPONENT;
    this->depthTexture->Filter_Min = this->depthTexture->Filter_Max = GL_NEAREST;
    this->depthTexture->GenerateFloat(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->depthTexture->ID, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::ShadowProcessor: Failed to initialize FBO" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowProcessor::~ShadowProcessor()
{
    glDeleteFramebuffers(1, &this->FBO);
    if (this->depthTexture != nullptr)
        delete this->depthTexture;
}

void ShadowProcessor::render(Scene *scene)
{
    glViewport(0, 0, this->Width, this->Height);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);


}