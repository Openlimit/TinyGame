#pragma once
#include "OpenGL_Common.h"

#include "Scene.h"
#include "TextureCubeMap.h"

class ShadowProcessor
{
public:
	GLuint Width;
	GLuint Height;

	Texture2D* directionDepthTexture;
	Shader* directionLightShadowShader;
	glm::mat4 directionLightSpaceMatrix;

	std::vector<TextureCubeMap*> pointDepthTextures;
	std::vector<float> far_planes;
	Shader* pointLightShadowShader;

	ShadowProcessor(GLuint width, GLuint height);

	~ShadowProcessor();

	void renderDirectionLight(Scene* scene);

	void renderPointLight(Scene* scene);

	void renderDepth();

private:
	GLuint directionLightFBO;
	GLuint pointLightFBO;
	GLuint VAO, VBO;
	Shader* shadowDebugShader;
};

