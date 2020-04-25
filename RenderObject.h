#pragma once
#include <glad/glad.h>
#include <vector>

#include "Texture2D.h"
#include "Shader.h"
#include "Mesh.h"

class RenderObject
{
public:
	Mesh* mesh;
	Shader shader;
	std::vector<Texture2D*> textures;
    GLuint VAO;

	RenderObject(Mesh* mesh, Shader shader, std::vector<Texture2D*>& textures) :mesh(mesh), shader(shader), textures(textures)
	{
        setup();
	}

    RenderObject(Mesh* mesh, Shader shader) :mesh(mesh), shader(shader)
    {
        setup();
    }

    ~RenderObject() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);

        if (mesh != nullptr)
            delete mesh;

        for (int i = 0; i < textures.size(); i++)
        {
            if (textures[i] != nullptr)
                delete textures[i];
        }
    }

private:
    GLuint VBO, EBO;

    void setup()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), &mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), &mesh->indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glBindVertexArray(0);
    }
};
