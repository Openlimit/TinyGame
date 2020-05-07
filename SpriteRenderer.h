#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture2D.h"
#include "Shader.h"


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader* shader, Shader* instanceShader, GLuint max_batch_size = 500);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

    void DrawSpriteBatch(Texture2D* texture, int batch_size, glm::vec2* positions, glm::vec2* sizes, GLfloat* rotates, glm::vec3* colors);
private:
    // Render state
    Shader* shader;
    Shader* batchShader;
    GLuint quadVAO;
    GLuint colorVBO;
    GLuint modelVBO;
    GLuint max_batch_size;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

