#pragma once
#include "Texture.h"

class Texture2D:public Texture
{
public:
    Texture2D();

    ~Texture2D();

    void Bind() const override;

    // Generates texture from image data
    void Generate(GLuint width, GLuint height, unsigned char* data);

    void GenerateFloat(GLuint width, GLuint height, float* data);

    void GenerateFloat(GLuint width, GLuint height, GLfloat* borderColor, float* data);

};

