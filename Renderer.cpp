#include "Renderer.h"

Renderer::Renderer()
{}

Renderer::~Renderer(){}

void Renderer::Draw(Mesh& mesh, Shader& shader, Texture2D& texture)
{
    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
    glBindVertexArray(0);
}

void Renderer::Draw(Mesh& mesh, Shader& shader)
{
    shader.Use();

    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
    glBindVertexArray(0);
}