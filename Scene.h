#pragma once
#include <map>
#include <string>

#include "RenderObject.h"

class Scene
{
public:
	std::map<std::string, RenderObject*> renderObjects;

	Scene() {}
	~Scene()
	{
		for (auto iter : renderObjects)
		{
			if (iter.second != nullptr)
				delete iter.second;
		}
	}

	void addRenderObject(Mesh* mesh, Shader* shader, std::string name)
	{
		auto obj = new RenderObject(mesh, shader);
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

	void update_VP(glm::mat4 view, glm::mat4 projection)
	{
		for (auto iter : renderObjects)
		{
			Shader* fShader = iter.second->forwardShader;
			Shader* dSahder = iter.second->defferedGeoShader;
			if (fShader != nullptr && fShader->auto_update_VP)
			{
				fShader->Use();
				fShader->SetMatrix4("view", view);
				fShader->SetMatrix4("projection", projection);
			}
			if (dSahder != nullptr && dSahder->auto_update_VP)
			{
				dSahder->Use();
				dSahder->SetMatrix4("view", view);
				dSahder->SetMatrix4("projection", projection);
			}
		}
	}
};
