#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer()
{}

ForwardRenderer::~ForwardRenderer()
{}

void ForwardRenderer::Draw(Scene* scene)
{
    if (postProcessor != nullptr)
        postProcessor->BeginRender();

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

        glBindVertexArray(renderObject->VAO);
        //glDrawElements(GL_TRIANGLES, renderObject->mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, renderObject->mesh->vertices.size());
        glBindVertexArray(0);
    }

    if (postProcessor != nullptr)
        postProcessor->Render();
}