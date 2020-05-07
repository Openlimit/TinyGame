#pragma once
#include <vector>
#include "OpenGL_Common.h"


class Mesh {
public:
    /*  Mesh Data  */
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> text_coords;
    std::vector<GLuint> indices;
    std::vector<GLuint> normal_indices;
    std::vector<GLuint> text_indices;

    /*  Functions  */
    // constructor
    Mesh() {}

    Mesh(std::vector<glm::vec3>& vertices, std::vector<GLuint>& indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }
};

class Quad : public Mesh
{
public:
    enum QUADCLASS
    {
        XY, ZY, XZ,
    };

    Quad(float left, float right, float down, float top, float height, QUADCLASS quadClass)
    {
        this->indices.resize(6);
        this->indices[0] = 3;
        this->indices[1] = 2;
        this->indices[2] = 0;
        this->indices[3] = 2;
        this->indices[4] = 1;
        this->indices[5] = 0;

        this->vertices.resize(4);
        switch (quadClass)
        {
        case XY:
            this->vertices[0] = glm::vec3(left, down, height);
            this->vertices[1] = glm::vec3(right, down, height);
            this->vertices[2] = glm::vec3(right, top, height);
            this->vertices[3] = glm::vec3(left, top, height);
            break;
        case ZY:
            this->vertices[0] = glm::vec3(height, left, down);
            this->vertices[1] = glm::vec3(height, right, down);
            this->vertices[2] = glm::vec3(height, right, top);
            this->vertices[3] = glm::vec3(height, left, top);
            break;
        case XZ:
            this->vertices[0] = glm::vec3(left, height, down);
            this->vertices[1] = glm::vec3(right, height, down);
            this->vertices[2] = glm::vec3(right, height, top);
            this->vertices[3] = glm::vec3(left, height, top);
            break;
        default:
            break;
        }
    }
};

