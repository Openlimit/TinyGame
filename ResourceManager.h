#pragma once
#include <map>
#include <string>

#include "Texture2D.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "TextureCubeMap.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader*>       Shaders;
    static std::map<std::string, Texture*>      Textures;
    static std::map<std::string, Mesh*>         Meshes;
    static std::map<std::string, Material*>     Materials;

    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader* LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
    // Retrieves a stored sader
    static Shader* GetShader(std::string name);
    // Loads (and generates) a texture from file
    static Texture* LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
    // Retrieves a stored texture
    static Texture* GetTexture(std::string name);

    static Texture* LoadTextureCubeMap(const GLchar* files[6], std::string name);
    
    static Texture* GetTextureCubeMap(std::string name);

    static Mesh* LoadMesh(const GLchar* file, std::string name);

    static Mesh* GetMesh(std::string name);

    static Material* LoadMaterial(Material::MaterialType type, std::string name);

    static Material* GetMaterial(std::string name);

    static void ReplaceMaterial(std::string name, Material* material);

    // Properly de-allocates all loaded resources
    static void      Clear();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // Loads and generates a shader from file
    static Shader* loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
    // Loads a single texture from file
    static Texture2D* loadTextureFromFile(const GLchar* file, GLboolean alpha);

    static TextureCubeMap* loadTextureCubeMapFromFile(const GLchar* files[6]);
};


