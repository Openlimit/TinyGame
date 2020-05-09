#pragma once
#include <map>
#include <vector>
#include <string>

#include "RenderObject.h"
#include "Light.h"

class Scene
{
public:
	std::map<std::string, RenderObject*> renderObjects;
	DirectionLight directionLight;
	std::vector<PointLight> lights;

	Scene() {}
	~Scene()
	{
		for (auto iter : renderObjects)
		{
			if (iter.second != nullptr)
				delete iter.second;
		}
	}

	void addRenderObject(Mesh* mesh, Material* material, Transform transform, std::string name)
	{
		auto obj = new RenderObject(mesh, material, transform);
		renderObjects[name] = obj;
	}

	void addRenderObject(Mesh* mesh, Material* material, std::string name)
	{
		auto obj = new RenderObject(mesh, material);
		renderObjects[name] = obj;
	}

	void addRenderObject(RenderObject* obj, std::string name)
	{
		renderObjects[name] = obj;
	}

	RenderObject* getRenderObject(std::string name)
	{
		if (renderObjects.find(name) != renderObjects.end())
			return renderObjects[name];
		else
			return nullptr;
	}
};
