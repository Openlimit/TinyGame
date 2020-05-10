#pragma once
#include <map>
#include <vector>
#include <string>

#include "RenderObject.h"
#include "Light.h"
#include "BBox.h"

class Scene
{
public:
	std::map<std::string, RenderObject*> renderObjects;
	DirectionLight directionLight;
	std::vector<PointLight> lights;
	BBox sceneBox;

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
		updateBBox(mesh, transform);
	}

	void addRenderObject(Mesh* mesh, Material* material, std::string name)
	{
		auto obj = new RenderObject(mesh, material);
		renderObjects[name] = obj;
		updateBBox(mesh, obj->transform);
	}

	void addRenderObject(RenderObject* obj, std::string name)
	{
		renderObjects[name] = obj;
		updateBBox(obj->mesh, obj->transform);
	}

	RenderObject* getRenderObject(std::string name)
	{
		if (renderObjects.find(name) != renderObjects.end())
			return renderObjects[name];
		else
			return nullptr;
	}

private:
	void updateBBox(Mesh *addMesh, Transform transform)
	{
		BBox meshBox = BBox::getBox(addMesh);
		glm::mat4 transMat = transform.getTransform();
		auto bPositions = meshBox.getBPosition();
		for (int i = 0; i < bPositions.size(); i++)
		{
			bPositions[i] = glm::vec3(transMat * glm::vec4(bPositions[i], 1));
		}
		meshBox = BBox::getBox(bPositions);

		if (renderObjects.empty())
		{
			sceneBox = meshBox;
		}
		else
		{
			sceneBox = BBox::merge(sceneBox, meshBox);
		}
	}
};
