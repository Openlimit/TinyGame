#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Instantiate static variables
std::map<std::string, Texture*>       ResourceManager::Textures;
std::map<std::string, Shader*>        ResourceManager::Shaders;
std::map<std::string, Mesh*>          ResourceManager::Meshes;
std::map<std::string, Material*>      ResourceManager::Materials;

Shader* ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader* ResourceManager::GetShader(std::string name)
{
    if (Shaders.find(name) != Shaders.end())
        return Shaders[name];
    else
        return nullptr;
}

Texture* ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture* ResourceManager::GetTexture(std::string name)
{
    if (Textures.find(name) != Textures.end())
        return Textures[name];
    else
        return nullptr;
}

void ResourceManager::Clear()
{	
    for (auto iter : Shaders)
        if (iter.second != nullptr)
            delete iter.second;
    for (auto iter : Textures)
        if (iter.second != nullptr)
            delete iter.second;
    for (auto iter : Meshes)
        if (iter.second != nullptr)
            delete iter.second;
    for (auto iter : Materials)
        if (iter.second != nullptr)
            delete iter.second;
}

Shader* ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader* shader = new Shader();
    shader->Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D* ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha)
{
    // Create Texture object
    Texture2D* texture = new Texture2D();
    if (alpha)
    {
        texture->Internal_Format = GL_RGBA;
        texture->Image_Format = GL_RGBA;
    }
    // Load image
    int width, height, nrChannels;
    unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);
    if (image == nullptr) {
        std::cout << "load texture fail:" << file << std::endl;
        return texture;
    }
    // Now generate texture
    texture->Generate(width, height, image);
    // And finally free image data
    stbi_image_free(image);
    return texture;
}

Texture* ResourceManager::LoadTextureCubeMap(const GLchar* files[6], std::string name)
{
    Textures[name] = loadTextureCubeMapFromFile(files);
    return Textures[name];
}

Texture* ResourceManager::GetTextureCubeMap(std::string name)
{
    if (Textures.find(name) != Textures.end())
        return Textures[name];
    else
        return nullptr;
}

TextureCubeMap* ResourceManager::loadTextureCubeMapFromFile(const GLchar* files[6])
{
    unsigned char* datas[6];
    int width, height, nrChannels;
    for (int i = 0; i < 6; i++)
    {
        datas[i] = stbi_load(files[i], &width, &height, &nrChannels, 0);
        if (!datas[i])
        {
            std::cout << "Cubemap texture failed to load at path: " << files[i] << std::endl;
            stbi_image_free(datas[i]);
            exit(-1);
        }
        if (nrChannels != 3)
        {
            std::cout << "Cubemap texture nrChannels is not 3: " << files[i] << std::endl;
            stbi_image_free(datas[i]);
            exit(-1);
        }
    }

    auto texture = new TextureCubeMap();
    texture->Wrap_S = texture->Wrap_T = texture->Wrap_R = GL_CLAMP_TO_EDGE;
    texture->Generate(width, height, datas);

    for (int i = 0; i < 6; i++)
        stbi_image_free(datas[i]);

    return texture;
}

Mesh* ResourceManager::LoadMesh(const GLchar* file, std::string name)
{
    std::ifstream in(file, std::ios::in);
    if (in.fail()) return nullptr;

    Mesh* mesh = new Mesh();
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> text_coords;
    std::vector<glm::vec3> normals;
    int count = -1;
    while (!in.eof()) 
    {
        std::string line;
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) 
        {
            iss >> trash;
            glm::vec3 vec;
            iss >> vec[0] >> vec[1] >> vec[2];
            vertices.emplace_back(vec);
        }
        else if (!line.compare(0, 3, "vt ")) 
        {
            iss >> trash >> trash;
            glm::vec3 vec;
            int i = 0;
            float v;
            while (iss >> v)
            {
                vec[i++] = v;
            }
            if (i == 2)
                vec[2] = 0;
            text_coords.emplace_back(vec);
        }
        else if (!line.compare(0, 3, "vn ")) 
        {
            iss >> trash >> trash;
            glm::vec3 vec;
            iss >> vec[0] >> vec[1] >> vec[2];
            normals.emplace_back(vec);
        }
        else if (!line.compare(0, 2, "f ")) 
        {
            if (count < 0)
            {
                count = 0;
                for (auto iter : line)
                {
                    if (iter == '/')
                        count++;
                }
                count /= 3;
                assert(count < 3);
            }
            
            iss >> trash;
            int id, t_id, n_id;
            for (int i = 0; i < 3; i++)
            {
                iss >> id;
                Mesh::Vertex vertex;
                vertex.Position = vertices[id - 1];

                if (count > 0)
                {
                    iss >> trash >> t_id;
                    vertex.TexCoords = text_coords[t_id - 1];
                }
                if (count > 1)
                {
                    iss >> trash >> n_id;
                    vertex.Normal = normals[n_id - 1];
                }

                mesh->vertices.emplace_back(vertex);
            }
        }
    }
    Meshes[name] = mesh;
    return mesh;
}

Mesh* ResourceManager::GetMesh(std::string name)
{
    if (Meshes.find(name) != Meshes.end())
        return Meshes[name];
    else
        return nullptr;
}

Material* ResourceManager::LoadMaterial(Material::MaterialType type, std::string name)
{
    switch (type)
    {
    case Material::MaterialType::DIFFUSE:
    {
        auto diffuseShader = GetShader("diffuse");
        if (diffuseShader == nullptr)
            diffuseShader = LoadShader("shaders/diffuse.vs", "shaders/diffuse.frag", nullptr, "diffuse");
        diffuseShader->auto_update_VP = true;
        auto diffuseMaterial = new DiffuseMaterial();
        diffuseMaterial->forwardShader = diffuseShader;
        Materials[name] = diffuseMaterial;
    }
    break;
    case Material::MaterialType::SKYBOX:
    {
        auto skyboxShader = GetShader("skybox");
        if (skyboxShader == nullptr)
            skyboxShader = ResourceManager::LoadShader("shaders/skybox.vs", "shaders/skybox.frag", nullptr, "skybox");
        skyboxShader->auto_update_VP = true;

        auto skyboxTexture = GetTextureCubeMap("skybox");
        if (skyboxTexture == nullptr)
        {
            const char* skyBoxFiles[] = {
            "resources/skybox/right.jpg",
            "resources/skybox/left.jpg",
            "resources/skybox/top.jpg",
            "resources/skybox/bottom.jpg",
            "resources/skybox/front.jpg",
            "resources/skybox/back.jpg"
            };
            skyboxTexture = LoadTextureCubeMap(skyBoxFiles, "skybox");
        }

        auto skyboxMaterial = new Material();
        skyboxMaterial->forwardShader = skyboxShader;
        skyboxMaterial->textures.emplace_back(skyboxTexture);
        Materials[name] = skyboxMaterial;
    }
    break;
    case Material::MaterialType::PBR:
    {
        auto pbrShader = GetShader("pbr");
        if (pbrShader == nullptr)
            pbrShader = ResourceManager::LoadShader("shaders/pbr.vs", "shaders/pbr.frag", nullptr, "pbr");
        pbrShader->auto_update_VP = true;
        auto pbrMaterial = new PBRMaterial();
        pbrMaterial->forwardShader = pbrShader;
        Materials[name] = pbrMaterial;
    }
    break;
    }

    return Materials[name];
}

Material* ResourceManager::GetMaterial(std::string name)
{
    if (Materials.find(name) != Materials.end())
        return Materials[name];
    else
        return nullptr;
}