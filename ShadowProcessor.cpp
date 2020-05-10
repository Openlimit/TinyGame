#include "ShadowProcessor.h"
#include <iostream>

#include "ResourceManager.h"

ShadowProcessor::ShadowProcessor(GLuint width, GLuint height):Width(width),Height(height)
{
    // Initializeframebuffer object
    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    this->depthTexture = new Texture2D();
    this->depthTexture->Image_Format = this->depthTexture->Internal_Format = GL_DEPTH_COMPONENT;
    this->depthTexture->Filter_Min = this->depthTexture->Filter_Max = GL_NEAREST;
    this->depthTexture->GenerateFloat(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture->ID, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::ShadowProcessor: Failed to initialize FBO" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->shadowShader = ResourceManager::LoadShader("shaders/shadow_map.vs", "shaders/shadow_map.frag", nullptr, "shadow_map");
}

ShadowProcessor::~ShadowProcessor()
{
    glDeleteFramebuffers(1, &this->FBO);
    if (this->depthTexture != nullptr)
        delete this->depthTexture;
}

void ShadowProcessor::render(Scene *scene)
{
    auto view = glm::mat3(glm::lookAt(glm::vec3(0), scene->directionLight.direction, glm::vec3(0, 1, 0)));
    auto bPositions = scene->sceneBox.getBPosition();
    glm::vec3 min_pos(INT_MAX);
    glm::vec3 max_pos(INT_MIN);
    for (auto pos : bPositions)
    {
        pos = view * pos;
        min_pos = glm::min(min_pos, pos);
        max_pos = glm::max(max_pos, pos);
    }

    //OpenGL进行view变换后，相机是朝向z轴负方向,所以要使用max_pos的z值
    glm::vec3 lightPos((min_pos.x + max_pos.x) / 2, (min_pos.y + max_pos.y) / 2, max_pos.z + 0.1);
    lightPos = glm::transpose(view) * lightPos;
    auto lightView = glm::lookAt(lightPos, lightPos + scene->directionLight.direction, glm::vec3(0, 1, 0));

    float half_width = (max_pos.x - min_pos.x) / 2;
    float half_height = (max_pos.y - min_pos.y) / 2;
    float depth = max_pos.z - min_pos.z + 0.1;
    auto projection = glm::ortho(-half_width, half_width, -half_height, half_height, 0.f, depth);
    this->lightSpaceMatrix = projection * lightView;

    glViewport(0, 0, this->Width, this->Height);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    this->shadowShader->Use();
    this->shadowShader->SetMatrix4("lightSpaceMatrix", this->lightSpaceMatrix);
    for (auto iter : scene->renderObjects)
    {
        auto renderObject = iter.second;
        if (!renderObject->material->isCastShadow)
            continue;

        glm::mat4 model = renderObject->transform.getTransform();
        this->shadowShader->SetMatrix4("model", model);

        glBindVertexArray(renderObject->VAO);
        glDrawArrays(GL_TRIANGLES, 0, renderObject->mesh->vertices.size());
        glBindVertexArray(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowProcessor::renderDepth()
{
    if (this->shadowDebugShader == nullptr)
    {
        GLfloat vertices[] = {
            // Pos        // Tex
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        this->shadowDebugShader = ResourceManager::LoadShader("shaders/screen_quad.vs", "shaders/debug_depth_map.frag", nullptr, "shadow_debug");
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->shadowDebugShader->Use();
    this->shadowDebugShader->SetInteger("depthMap", 0);

    // Render textured quad
    glActiveTexture(GL_TEXTURE0);
    this->depthTexture->Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}