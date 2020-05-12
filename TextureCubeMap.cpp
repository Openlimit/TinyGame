#include "TextureCubeMap.h"

TextureCubeMap::TextureCubeMap() 
{
}

TextureCubeMap::~TextureCubeMap()
{
}

void TextureCubeMap::Bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
}

void TextureCubeMap::Generate(GLuint width, GLuint height, unsigned char* datas[6])
{
	assert(width == height);
	Width = width;
	Height = height;

	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, datas[i]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Filter_Max);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Filter_Min);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Wrap_S);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Wrap_T);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Wrap_R);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::GenerateFloat(GLuint width, GLuint height, float* datas[6])
{
	assert(width == height);
	Width = width;
	Height = height;

	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Internal_Format, width, height, 0, Image_Format, GL_FLOAT, datas[i]);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Filter_Max);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Filter_Min);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Wrap_S);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Wrap_T);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Wrap_R);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}