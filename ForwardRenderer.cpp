#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer(GLuint width, GLuint height) :Renderer(width, height)
{}

ForwardRenderer::~ForwardRenderer()
{}

void ForwardRenderer::Init(Scene* scene)
{
    if (this->shadowProcessor != nullptr)
    {
        this->shadowProcessor->renderDirectionLight(scene);
        this->shadowProcessor->renderPointLight(scene);
    }
}

void ForwardRenderer::Draw(Scene* scene)
{
    if (this->postProcessor != nullptr)
        this->postProcessor->BeginRender();

    glViewport(0, 0, Width, Height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto iter : scene->shaderObjectlist)
    {
        auto shaderID = iter.first;
        auto objectList = iter.second;

        glUseProgram(shaderID);
        for (auto obj: objectList)
        {
            glm::mat4 model = obj->transform.getTransform();
            obj->material->forwardShader->SetMatrix4("model", model);
            obj->material->updateForwardShader();
            for (int j = 0; j < obj->material->textures.size(); j++)
            {
                glActiveTexture(GL_TEXTURE0 + j);
                obj->material->textures[j]->Bind();
            }

            if (this->shadowProcessor != nullptr && obj->material->isReceiveShadow)
            {
                GLuint texture_id = obj->material->textures.size();
                glActiveTexture(GL_TEXTURE0 + texture_id);
                this->shadowProcessor->directionDepthTexture->Bind();
                obj->material->forwardShader->SetInteger("directionShadowMap", texture_id);
                obj->material->forwardShader->SetMatrix4("lightSpaceMatrix", this->shadowProcessor->directionLightSpaceMatrix);
                
                if (scene->pointLights.size() > 0)
                {
                    glActiveTexture(GL_TEXTURE0 + texture_id + 1);
                    this->shadowProcessor->pointDepthTextures[0]->Bind();
                    obj->material->forwardShader->SetInteger("pointShadowMap", texture_id + 1);
                    obj->material->forwardShader->SetFloat("far_plane", this->shadowProcessor->far_planes[0]);
                }
            }

            glBindVertexArray(obj->VAO);
            //glDrawElements(GL_TRIANGLES, obj->mesh->indices.size(), GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, obj->mesh->vertices.size());
            glBindVertexArray(0);
        }
    }

    if (scene->skyBox != nullptr)
    {
        glDepthFunc(GL_LEQUAL);

        scene->skyBox->material->forwardShader->Use();
        glActiveTexture(GL_TEXTURE0);
        scene->skyBox->material->textures[0]->Bind();
        glBindVertexArray(scene->skyBox->VAO);
        glDrawArrays(GL_TRIANGLES, 0, scene->skyBox->mesh->vertices.size());
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
    }

    if (this->postProcessor != nullptr)
        this->postProcessor->Render();
}