#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer()
{}

ForwardRenderer::~ForwardRenderer()
{}

void ForwardRenderer::Draw(std::vector<RenderObject*>& renderObjects)
{
    if (postProcessor != nullptr)
        postProcessor->BeginRender();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < renderObjects.size(); i++)
    {
        renderObjects[i]->forwardShader.Use();
        for (int j = 0; j < renderObjects[i]->textures.size(); j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            renderObjects[i]->textures[j]->Bind();
        }

        glBindVertexArray(renderObjects[i]->VAO);
        glDrawElements(GL_TRIANGLES, renderObjects[i]->mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    if (postProcessor != nullptr)
        postProcessor->Render();
}

void ForwardRenderer::Draw(RenderObject* renderObject)
{
    if (postProcessor != nullptr)
        postProcessor->BeginRender();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderObject->forwardShader.Use();
    for (int i = 0; i < renderObject->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        renderObject->textures[i]->Bind();
    }

    glBindVertexArray(renderObject->VAO);
    glDrawElements(GL_TRIANGLES, renderObject->mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (postProcessor != nullptr)
        postProcessor->Render();
}