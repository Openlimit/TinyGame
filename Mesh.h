#pragma once
#include <vector>
#include "OpenGL_Common.h"


class Mesh {
public:
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 TexCoords;
    };

    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    /*  Functions  */
    // constructor
    Mesh() {}

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }
};

