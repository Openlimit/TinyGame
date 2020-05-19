#pragma once
#include <vector>

#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "BSphere.h"

class RenderObject
{
public:
	Mesh* mesh;
    Transform transform;
    Material* material;
    GLuint VAO;
    bool visible;

	RenderObject(Mesh* mesh, Material* material, Transform transform) :mesh(mesh), transform(transform), material(material)
	{
        setup();
	}

    RenderObject(Mesh* mesh, Material* material) :mesh(mesh), material(material)
    {
        setup();
    }

    RenderObject(Mesh* mesh) :mesh(mesh)
    {
        setup();
    }

    virtual ~RenderObject() {
        glDeleteBuffers(1, &VBO);
        //glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    BSphere getBounding()
    {
        glm::mat4 transMat = transform.getTransform();
        BSphere curBounding;
        curBounding.center = glm::vec3(transMat * glm::vec4(bounding.center, 1.f));
        curBounding.radius = bounding.radius * std::fmax(std::fmax(transform.scale.x, transform.scale.y), transform.scale.z);
        return curBounding;
    }

private:
    GLuint VBO, EBO;
    BSphere bounding;

    void setup()
    {
        bounding = BSphere::getBSphere(mesh);

        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh::Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), &mesh->indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, TexCoords));

        glBindVertexArray(0);
    }
};
