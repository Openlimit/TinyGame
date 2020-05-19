#include "ShadowProcessor.h"
#include <iostream>

#include "ResourceManager.h"
#include "OrthCamera.h"

BSphere GetShadowBounding(Scene* scene)
{
    BSphere bounding;
    bounding.radius = 0;
    for (auto iter : scene->renderObjects)
    {
        auto obj = iter.second;
        if (obj->visible && obj->material->isReceiveShadow)
        {
            if (bounding.radius > 0)
            {
                bounding = BSphere::merge(bounding, obj->getBounding());
            }
            else
            {
                bounding = obj->getBounding();
            }
        }
    }
    return bounding;
}

ShadowProcessor::ShadowProcessor(GLuint width, GLuint height):Width(width),Height(height)
{
    // Initialize Direction Light FrameBuffer
    glGenFramebuffers(1, &directionLightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, directionLightFBO);

    directionDepthTexture = new Texture2D();
    directionDepthTexture->Image_Format = directionDepthTexture->Internal_Format = GL_DEPTH_COMPONENT;
    directionDepthTexture->Filter_Min = directionDepthTexture->Filter_Max = GL_NEAREST;
    directionDepthTexture->Wrap_S = directionDepthTexture->Wrap_T = GL_CLAMP_TO_BORDER;
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    directionDepthTexture->GenerateFloat(width, height, borderColor, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, directionDepthTexture->ID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::ShadowProcessor: Failed to initialize directionLightFBO" << std::endl;

    // Initialize Point Light FrameBuffer
    glGenFramebuffers(1, &pointLightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);

    auto pointDepthTexture = new TextureCubeMap();
    pointDepthTexture->Image_Format = pointDepthTexture->Internal_Format = GL_DEPTH_COMPONENT;
    pointDepthTexture->Filter_Min = pointDepthTexture->Filter_Max = GL_NEAREST;
    pointDepthTexture->Wrap_S = pointDepthTexture->Wrap_T = pointDepthTexture->Wrap_R = GL_CLAMP_TO_EDGE;
    float* data[6] = { nullptr };
    pointDepthTexture->GenerateFloat(height, height, data);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointDepthTexture->ID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::ShadowProcessor: Failed to initialize pointLightFBO " << std::endl;
    pointDepthTextures.emplace_back(pointDepthTexture);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    directionLightShadowShader = ResourceManager::LoadShader("shaders/shadow_map.vs", "shaders/shadow_map.frag", nullptr, "shadow_map");
    pointLightShadowShader = ResourceManager::LoadShader("shaders/omni_shadow_map.vs", "shaders/omni_shadow_map.frag", "shaders/omni_shadow_map.gs", "omni_shadow_map");
}

ShadowProcessor::~ShadowProcessor()
{
    glDeleteFramebuffers(1, &directionLightFBO);
    glDeleteFramebuffers(1, &pointLightFBO);
    if (directionDepthTexture != nullptr)
        delete directionDepthTexture;
    for (auto texture : pointDepthTextures)
    {
        if (texture != nullptr)
            delete texture;
    }
}

void ShadowProcessor::renderDirectionLight(Scene *scene)
{
    auto view = glm::mat3(glm::lookAt(glm::vec3(0), scene->directionLight.direction, glm::vec3(0, 1, 0)));
    BSphere bounding = GetShadowBounding(scene);
    bounding.center = view * bounding.center;

    //OpenGL进行view变换后，相机是朝向z轴负方向,所以要使用max_pos的z值
    float max_z = bounding.center.z + bounding.radius;
    for (auto iter : scene->renderObjects)
    {
        auto obj = iter.second;
        //需要考虑视野外的Cast Shadow物体
        if (obj->material->isCastShadow)
        {
            auto bs = obj->getBounding();
            bs.center = view * bs.center;
            if (bs.center.x - bs.radius < bounding.center.x + bounding.radius &&
                bs.center.x + bs.radius > bounding.center.x - bounding.radius &&
                bs.center.y - bs.radius < bounding.center.y + bounding.radius &&
                bs.center.y + bs.radius > bounding.center.y - bounding.radius &&
                bs.center.z + bs.radius > max_z)
            {
                max_z = bs.center.z + bs.radius;
            }
        }
    }

    glm::vec3 lightPos(bounding.center.x, bounding.center.y, max_z);
    lightPos = glm::transpose(view) * lightPos;
    auto lightView = glm::lookAt(lightPos, lightPos + scene->directionLight.direction, glm::vec3(0, 1, 0));
    auto projection = glm::ortho(-bounding.radius, bounding.radius, -bounding.radius, bounding.radius,
        0.f, max_z - (bounding.center.z - bounding.radius));
    directionLightSpaceMatrix = projection * lightView;

    glViewport(0, 0, Width, Height);
    glBindFramebuffer(GL_FRAMEBUFFER, directionLightFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    directionLightShadowShader->Use();
    directionLightShadowShader->SetMatrix4("lightSpaceMatrix", directionLightSpaceMatrix);
    for (auto iter : scene->renderObjects)
    {
        auto renderObject = iter.second;
        if (!renderObject->material->isCastShadow)
            continue;

        glm::mat4 model = renderObject->transform.getTransform();
        directionLightShadowShader->SetMatrix4("model", model);

        glBindVertexArray(renderObject->VAO);
        glDrawArrays(GL_TRIANGLES, 0, renderObject->mesh->vertices.size());
        glBindVertexArray(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowProcessor::renderPointLight(Scene* scene)
{
    if (scene->pointLights.size() == 0)
        return;

    BSphere bounding = GetShadowBounding(scene);

    glViewport(0, 0, Height, Height);
    glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    while (scene->pointLights.size() > pointDepthTextures.size())
    {
        auto pointDepthTexture = new TextureCubeMap();
        pointDepthTexture->Image_Format = pointDepthTexture->Internal_Format = GL_DEPTH_COMPONENT;
        pointDepthTexture->Filter_Min = pointDepthTexture->Filter_Max = GL_NEAREST;
        pointDepthTexture->Wrap_S = pointDepthTexture->Wrap_T = pointDepthTexture->Wrap_R = GL_CLAMP_TO_EDGE;
        float* data[6] = { nullptr };
        pointDepthTexture->GenerateFloat(Height, Height, data);
        pointDepthTextures.emplace_back(pointDepthTexture);
    }

    far_planes.resize(pointDepthTextures.size());

    pointLightShadowShader->Use();
    for (int i = 0; i < scene->pointLights.size(); i++)
    {
        auto pointLight = scene->pointLights[i];
        GLfloat far = 0;
        for (int j = 0; j < 3; j++)
        {
            far = std::fmax(far, std::fabs(bounding.center[j] + bounding.radius - pointLight.position[j]));
            far = std::fmax(far, std::fabs(bounding.center[j] - bounding.radius - pointLight.position[j]));
        }
        far_planes[i] = far;

        GLfloat aspect = 1.f;
        GLfloat near = 0.1f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.emplace_back(shadowProj *
            glm::lookAt(pointLight.position, pointLight.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        shadowTransforms.emplace_back(shadowProj *
            glm::lookAt(pointLight.position, pointLight.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        shadowTransforms.emplace_back(shadowProj *
            glm::lookAt(pointLight.position, pointLight.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        shadowTransforms.emplace_back(shadowProj *
            glm::lookAt(pointLight.position, pointLight.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
        shadowTransforms.emplace_back(shadowProj *
            glm::lookAt(pointLight.position, pointLight.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
        shadowTransforms.emplace_back(shadowProj *
            glm::lookAt(pointLight.position, pointLight.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

        for (GLuint j = 0; j < 6; ++j)
            pointLightShadowShader->SetMatrix4(("shadowMatrices[" + std::to_string(j) + "]").c_str(), shadowTransforms[j]);

        pointLightShadowShader->SetFloat("far_plane", far);
        pointLightShadowShader->SetVector3f("lightPos", pointLight.position);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointDepthTextures[i]->ID, 0);

        for (auto iter : scene->renderObjects)
        {
            auto renderObject = iter.second;
            if (!renderObject->material->isCastShadow)
                continue;

            glm::mat4 model = renderObject->transform.getTransform();
            pointLightShadowShader->SetMatrix4("model", model);

            glBindVertexArray(renderObject->VAO);
            glDrawArrays(GL_TRIANGLES, 0, renderObject->mesh->vertices.size());
            glBindVertexArray(0);
        }
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
    directionDepthTexture->Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}