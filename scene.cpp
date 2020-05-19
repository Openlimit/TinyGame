#include "Scene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

void Scene::updateShaderObjectlist(RenderObject* obj)
{
    if (obj->material == nullptr)
        return;

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
        Material* material = nullptr;
        if (object.HasMember("material"))
        {
            rapidjson::Value& materialObj = object["material"];
            std::cout << "materialName: " << materialObj["name"].GetString() << std::endl;
            std::string type(materialObj["type"].GetString());
            Material::MaterialType mtype;
            if (type == "DIFFUSE")
                mtype = Material::MaterialType::DIFFUSE;
            else if (type == "PBR")
                mtype = Material::MaterialType::PBR;
            else
            {
                std::cout << "MaterialType not exist: " << type << std::endl;
                exit(-1);
            }

            material = ResourceManager::LoadMaterial(mtype, materialObj["name"].GetString());
            if (materialObj.HasMember("parameters"))
            {
                rapidjson::Value& parameters = materialObj["parameters"];
                switch (mtype)
                {
                case Material::MaterialType::DIFFUSE:
                {
                    if (!parameters.HasMember("diffuse_color"))
                    {
                        std::cout << "DIFFUSE diffuse_color not exist: " << materialObj["name"].GetString() << std::endl;
                        exit(-1);
                    }
                    auto diffuse = dynamic_cast<DiffuseMaterial*>(material);
                    rapidjson::Value& diffuse_color = parameters["diffuse_color"];
                    diffuse->diffuse_color = glm::vec3(diffuse_color[0].GetFloat(),
                        diffuse_color[1].GetFloat(),
                        diffuse_color[2].GetFloat());
                }
                break;
                case Material::MaterialType::PBR:
                {
                    if (!parameters.HasMember("albedo") || !parameters.HasMember("metallic") ||
                        !parameters.HasMember("roughness") || !parameters.HasMember("ao"))
                    {
                        std::cout << "PBR parameters not complete: " << materialObj["name"].GetString() << std::endl;
                        exit(-1);
                    }
                    auto pbr = dynamic_cast<PBRMaterial*>(material);
                    rapidjson::Value& albedo = parameters["albedo"];
                    pbr->albedo = glm::vec3(albedo[0].GetFloat(),
                        albedo[1].GetFloat(),
                        albedo[2].GetFloat());
                    pbr->metallic = parameters["metallic"].GetFloat();
                    pbr->roughness = parameters["roughness"].GetFloat();
                    pbr->ao = parameters["ao"].GetFloat();
                }
                break;
                default:
                    break;
                }
            }

            if (materialObj.HasMember("isCastShadow"))
                material->isCastShadow = materialObj["isCastShadow"].GetBool();
            if (materialObj.HasMember("isReceiveShadow"))
                material->isReceiveShadow = materialObj["isReceiveShadow"].GetBool();
        }

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