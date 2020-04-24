#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture2D.h"
#include "Shader.h"
#include "Mesh.h"

class Renderer
{
public:
    
    Renderer();
    
    ~Renderer();
    
    void Draw(Mesh& mesh, Shader& shader, Texture2D& texture);

    void Draw(Mesh& mesh, Shader& shader);
};

