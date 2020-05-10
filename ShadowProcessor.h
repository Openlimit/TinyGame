#pragma once
#include "OpenGL_Common.h"

#include "Scene.h"

class ShadowProcessor
{
public:
	GLuint Width;
	GLuint Height;
	Texture2D* depthTexture;
	Shader* shadowShader;
	glm::mat4 lightSpaceMatrix;

	ShadowProcessor(GLuint width, GLuint height);

	~ShadowProcessor();

	void render(Scene* scene);

	void renderDepth();

private:
	GLuint FBO;
	GLuint VAO, VBO;
	Shader* shadowDebugShader;
};

