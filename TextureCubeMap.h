#pragma once
#include "Texture.h"

class TextureCubeMap:public Texture
{
public:
	TextureCubeMap();

	~TextureCubeMap();

	void Bind() const override;

	void Generate(GLuint width, GLuint height, unsigned char* datas[6]);

	void GenerateFloat(GLuint width, GLuint height, float* datas[6]);
};