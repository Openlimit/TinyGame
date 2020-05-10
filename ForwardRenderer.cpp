#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer()
{}

ForwardRenderer::~ForwardRenderer()
{}

void ForwardRenderer::Draw(Scene* scene)
{
    if (this->shadowProcessor != nullptr) 
    {
        this->shadowProcessor->render(scene);
        //this->shadowProcessor->renderDepth();
        //return;
    }

    if (this->postProcessor != nullptr)
        this->postProcessor->BeginRender();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto iter : scene->renderObjects)
    {
        auto renderObject = iter.second;
        renderObject->material->forwardShader->Use();
        glm::mat4 model = renderObject->transform.getTransform();
        renderObject->material->forwardShader->SetMatrix4("model", model);
        renderObject->material->updateForwardShader();
        for (int j = 0; j < renderObject->material->textures.size(); j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            renderObject->material->textures[j]->Bind();
        }

        if (this->shadowProcessor != nullptr && renderObject->material->isReceiveShadow)
        {
            GLuint texture_id = renderObject->material->textures.size();
            glActiveTexture(GL_TEXTURE0 + texture_id);
            this->shadowProcessor->depthTexture->Bind();
            renderObject->material->forwardShader->SetInteger("depthMap", texture_id);
            renderObject->material->forwardShader->SetMatrix4("lightSpaceMatrix", this->shadowProcessor->lightSpaceMatrix);
        }

        glBindVertexArray(renderObject->VAO);
        //glDrawElements(GL_TRIANGLES, renderObject->mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, renderObject->mesh->vertices.size());
        glBindVertexArray(0);
    }

    if (this->postProcessor != nullptr)
        this->postProcessor->Render();
}