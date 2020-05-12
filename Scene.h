#pragma once
#include <map>
#include <vector>
#include <string>

#include "RenderObject.h"
#include "Light.h"
#include "BSphere.h"
#include "ResourceManager.h"

class Scene
{
public:
	std::map<std::string, RenderObject*> renderObjects;
	std::map<GLuint, std::vector<RenderObject*>> shaderObjectlist;
	DirectionLight directionLight;
	std::vector<PointLight> pointLights;
	BSphere sceneBounding;

	RenderObject* skyBox;

	Scene() {}
	~Scene()
	{
		for (auto iter : renderObjects)
		{
			if (iter.second != nullptr)
				delete iter.second;
		}
		if (skyBox != nullptr)
			delete skyBox;
	}

	void addRenderObject(Mesh* mesh, Material* material, Transform transform, std::string name)
	{
		auto obj = new RenderObject(mesh, material, transform);
		updateSceneBounding(obj->bounding);
		updateShaderObjectlist(obj);
		renderObjects[name] = obj;
	}

	void addRenderObject(Mesh* mesh, Material* material, std::string name)
	{
		auto obj = new RenderObject(mesh, material);
		updateSceneBounding(obj->bounding);
		updateShaderObjectlist(obj);
		renderObjects[name] = obj;
	}

	void addRenderObject(RenderObject* obj, std::string name)
	{
		updateSceneBounding(obj->bounding);
		updateShaderObjectlist(obj);
		renderObjects[name] = obj;
	}

	RenderObject* getRenderObject(std::string name)
	{
		if (renderObjects.find(name) != renderObjects.end())
			return renderObjects[name];
		else
			return nullptr;
	}

	void addSkyBox()
	{
		auto mesh = ResourceManager::LoadMesh("resources/models/skybox.obj", "skybox");
		auto material = ResourceManager::LoadMaterial(Material::MaterialType::SKYBOX, "skybox");
		skyBox = new RenderObject(mesh, material);
	}

private:
	void updateSceneBounding(BSphere meshBounding)
	{
		if (renderObjects.empty())
		{
			sceneBounding = meshBounding;
		}
		else
		{
			sceneBounding = BSphere::merge(sceneBounding, meshBounding);
		}
	}

	void updateShaderObjectlist(RenderObject* obj)
	{
		GLuint shaderID;
		if (obj->material->forwardShader != nullptr)
			shaderID = obj->material->forwardShader->ID;
		else if (obj->material->defferedGeoShader != nullptr)
			shaderID = obj->material->defferedGeoShader->ID;
		else
			return;

		if (shaderObjectlist.find(shaderID) != shaderObjectlist.end())
		{
			shaderObjectlist[shaderID].emplace_back(obj);
		}
		else
		{
			std::vector<RenderObject*> list = { obj };
			shaderObjectlist[shaderID] = list;
		}
	}
};
