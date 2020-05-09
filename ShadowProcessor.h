#pragma once
#include "OpenGL_Common.h"

#include "Scene.h"

class ShadowProcessor
{
public:
	GLuint Width;
	GLuint Height;
	Texture2D* depthTexture;

	ShadowProcessor(GLuint width, GLuint height);

	~ShadowProcessor();

	void render(Scene* scene);

private:
	GLuint FBO;
};

