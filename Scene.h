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

	void load(std::string path);

	void addRenderObject(Mesh* mesh, Material* material, Transform transform, std::string name)
	{
		auto obj = new RenderObject(mesh, material, transform);
		updateShaderObjectlist(obj);
		renderObjects[name] = obj;
	}

	void addRenderObject(Mesh* mesh, Material* material, std::string name)
	{
		auto obj = new RenderObject(mesh, material);
		updateShaderObjectlist(obj);
		renderObjects[name] = obj;
	}

	void addRenderObject(RenderObject* obj, std::string name)
	{
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

	void addMaterial(std::string name, Material* material)
	{
		auto obj = renderObjects[name];
		if (obj == nullptr || obj->material != nullptr)
			return;
		obj->material = material;
		updateShaderObjectlist(obj);
	}

private:
	void updateShaderObjectlist(RenderObject* obj);
};
