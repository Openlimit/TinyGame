#include "Scene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

void Scene::updateShaderObjectlist(RenderObject* obj)
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

void Scene::load(std::string path)
{
    std::ifstream in(path);
    std::ostringstream tmp;
    tmp << in.rdbuf();

    rapidjson::Document document;
    if (document.Parse(tmp.str().c_str()).HasParseError())
    {
        std::cout << "LoadScene Fail: parse json failed" << std::endl;
        exit(-1);
    }

    std::cout << "SceneName: " << document["name"].GetString() << std::endl;

    //objects
    rapidjson::Value& objects = document["objects"];
    for (rapidjson::SizeType i = 0; i < objects.Size(); i++)
    {
        rapidjson::Value& object = objects[i];
        std::cout << "ObjectName: " << object["name"].GetString() << std::endl;

        //mesh
        rapidjson::Value& meshObj = object["mesh"];
        std::cout << "MeshName: " << meshObj["name"].GetString() << std::endl;
        auto mesh = ResourceManager::GetMesh(meshObj["name"].GetString());
        if (mesh == nullptr)
            mesh = ResourceManager::LoadMesh(meshObj["path"].GetString(), meshObj["name"].GetString());

        //material
        rapidjson::Value& materialObj = object["material"];
        std::cout << "materialName: " << materialObj["name"].GetString() << std::endl;
        std::string type(materialObj["type"].GetString());
        Material::MaterialType mtype;
        if (type == "DIFFUSE")
            mtype = Material::MaterialType::DIFFUSE;
        else if (type == "PBR")
            mtype = Material::MaterialType::PBR;
        else if (type == "CUSTOM")
            mtype = Material::MaterialType::CUSTOM;
        else
        {
            std::cout << "MaterialType not exist: " << type << std::endl;
            exit(-1);
        }
        
        Material* material;
        if (mtype == Material::MaterialType::CUSTOM)
        {
            material = new Material();
            ResourceManager::Materials[materialObj["name"].GetString()] = material;
        }
        else
        {
            material = ResourceManager::LoadMaterial(mtype, materialObj["name"].GetString());
        }
         
        if (materialObj.HasMember("isCastShadow"))
            material->isCastShadow = materialObj["isCastShadow"].GetBool();
        if (materialObj.HasMember("isReceiveShadow"))
            material->isReceiveShadow = materialObj["isReceiveShadow"].GetBool();

        //transform
        Transform transform;
        if (object.HasMember("transform"))
        {
            rapidjson::Value& transformObj = object["transform"];
            if (transformObj.HasMember("scale"))
            {
                rapidjson::Value& scale = transformObj["scale"];
                transform.scale = glm::vec3(scale[0].GetFloat(),
                    scale[1].GetFloat(),
                    scale[2].GetFloat());
            }
            if (transformObj.HasMember("translation"))
            {
                rapidjson::Value& translation = transformObj["translation"];
                transform.translation = glm::vec3(translation[0].GetFloat(),
                    translation[1].GetFloat(),
                    translation[2].GetFloat());
            }
        }

        addRenderObject(mesh, material, transform, object["name"].GetString());
    }

    //lights
    if (document.HasMember("lights"))
    {
        rapidjson::Value& lights = document["lights"];
        for (rapidjson::SizeType i = 0; i < lights.Size(); i++)
        {
            rapidjson::Value& lightObj = lights[i];
            std::string type(lightObj["type"].GetString());
            if (type == "DIRECTION")
            {
                DirectionLight light;
                rapidjson::Value& direction = lightObj["direction"];
                light.direction = glm::normalize(glm::vec3(direction[0].GetFloat(),
                    direction[1].GetFloat(),
                    direction[2].GetFloat()));
                rapidjson::Value& color = lightObj["color"];
                light.color = glm::vec3(color[0].GetFloat(),
                    color[1].GetFloat(),
                    color[2].GetFloat());
                directionLight = light;
            }
            else if (type == "POINT")
            {
                PointLight light;
                rapidjson::Value& position = lightObj["position"];
                light.position = glm::vec3(position[0].GetFloat(),
                    position[1].GetFloat(),
                    position[2].GetFloat());
                rapidjson::Value& color = lightObj["color"];
                light.color = glm::vec3(color[0].GetFloat(),
                    color[1].GetFloat(),
                    color[2].GetFloat());
                pointLights.emplace_back(light);
            }
            else
            {
                std::cout << "LightType not exist: " << type << std::endl;
                exit(-1);
            }
        }
        std::cout << "Loaded lights" << std::endl;
    }
}