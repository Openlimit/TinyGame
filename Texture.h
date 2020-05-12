#pragma once
#include "OpenGL_Common.h"

class Texture
{
public:
	GLuint ID;
	GLuint Width, Height;
	GLuint Internal_Format;
	GLuint Image_Format;
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Wrap_R;
	GLuint Filter_Min;
	GLuint Filter_Max;

	Texture() :Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB),
		Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Wrap_R(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
	{
		glGenTextures(1, &this->ID);
	}

	~Texture()
	{
		glDeleteTextures(1, &this->ID);
	}

	virtual void Bind() const = 0;
};