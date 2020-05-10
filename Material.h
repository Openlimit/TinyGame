#pragma once
#include "OpenGL_Common.h"
#include <vector>

#include "Camera.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Light.h"

class Material
{
public:
	enum class MaterialType
	{
		DIFFUSE
	};

	Shader* forwardShader;
	Shader* defferedGeoShader;
	Shader* defferedShadingShader;

	std::vector<Texture2D*> textures;

	bool isReceiveShadow;
	bool isCastShadow;

	Material() :isReceiveShadow(false), isCastShadow(false) {}

	//每帧调用，此时renderer已绑定shader
	virtual void updateForwardShader() {}

	virtual void updateDefferedGeoShader() {}

	virtual void updateDefferedShadingShader() {}

};

class DiffuseMaterial:public Material
{
public:
	glm::vec3 diffuse_color;
	Camera* camera;

	DiffuseMaterial() :Material()
	{}

	void setupForwardShader(DirectionLight directionLight, Camera* _camera, glm::vec3 _diffuse_color)
	{
		this->camera = _camera;
		this->diffuse_color = _diffuse_color;

		this->forwardShader->Use();
		this->forwardShader->SetVector3f("directionLight.direction", directionLight.direction);
		this->forwardShader->SetVector3f("directionLight.color", directionLight.color);
	}

	void updateForwardShader() override
	{
		this->forwardShader->SetVector3f("diffuse_color", diffuse_color);
		this->forwardShader->SetVector3f("viewPos", camera->Position);
	}
};