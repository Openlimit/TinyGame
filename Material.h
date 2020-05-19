#pragma once
#include "OpenGL_Common.h"
#include <vector>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Light.h"

class Material
{
public:
	enum class MaterialType
	{
		DIFFUSE,
		SKYBOX,
		PBR,
		CUSTOM
	};

	Shader* forwardShader;
	Shader* defferedGeoShader;
	Shader* defferedShadingShader;

	std::vector<Texture*> textures;

	bool isReceiveShadow;
	bool isCastShadow;
	MaterialType type;

	Material() :isReceiveShadow(false), isCastShadow(false), type(MaterialType::CUSTOM) {}

	Material(MaterialType type) :isReceiveShadow(false), isCastShadow(false), type(type) {}

	//每帧调用，此时renderer已绑定shader
	virtual void updateForwardShader() {}

	virtual void updateDefferedGeoShader() {}

	virtual void updateDefferedShadingShader() {}

};

class DiffuseMaterial:public Material
{
public:
	glm::vec3 diffuse_color;

	DiffuseMaterial() :Material(MaterialType::DIFFUSE)
	{}

	void setupForwardShader(DirectionLight directionLight, PointLight pointLight)
	{
		this->forwardShader->Use();
		this->forwardShader->SetVector3f("directionLight.direction", directionLight.direction);
		this->forwardShader->SetVector3f("directionLight.color", directionLight.color);
		this->forwardShader->SetVector3f("pointLight.position", pointLight.position);
		this->forwardShader->SetVector3f("pointLight.color", pointLight.color);
		this->forwardShader->SetFloat("receiveShadow", isReceiveShadow);
	}

	void updateForwardShader() override
	{
		this->forwardShader->SetVector3f("diffuse_color", diffuse_color);
	}
};

class PBRMaterial :public Material
{
public:
	glm::vec3 albedo;
	float metallic;
	float roughness;
	float ao;

	PBRMaterial() :Material(MaterialType::PBR)
	{}

	void setupForwardShader(std::vector<PointLight> &pointLights)
	{
		this->forwardShader->Use();
		for (int i = 0; i < pointLights.size(); i++)
		{
			this->forwardShader->SetVector3f(("lightPositions[" + std::to_string(i) + "]").c_str(), pointLights[i].position);
			this->forwardShader->SetVector3f(("lightColors[" + std::to_string(i) + "]").c_str(), pointLights[i].color);
		}
	}

	void updateForwardShader() override
	{
		this->forwardShader->SetVector3f("albedo", albedo);
		this->forwardShader->SetFloat("metallic", metallic);
		this->forwardShader->SetFloat("roughness", roughness);
		this->forwardShader->SetFloat("ao", ao);
	}
};