#include "RPGGame.h"
#include "FPSCamera.h"
#include <fstream>

void generateSphere()
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<int> indices;

    const int X_SEGMENTS = 64;
    const int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;
    for (int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0 * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0 * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (int x = 1; x <= X_SEGMENTS; ++x)
            {
                int n0 = y * (X_SEGMENTS + 1) + x - 1;
                int n1 = (y + 1) * (X_SEGMENTS + 1) + x - 1;
                int n2 = y * (X_SEGMENTS + 1) + x;
                int n3 = (y + 1) * (X_SEGMENTS + 1) + x;

                indices.emplace_back(n2);
                indices.emplace_back(n1);
                indices.emplace_back(n0);

                indices.emplace_back(n3);
                indices.emplace_back(n1);
                indices.emplace_back(n2);
            }
        }
        else
        {
            for (int x = X_SEGMENTS - 1; x >= 0; --x)
            {
                int n0 = (y + 1) * (X_SEGMENTS + 1) + x + 1;
                int n1 = y * (X_SEGMENTS + 1) + x + 1;
                int n2 = (y + 1) * (X_SEGMENTS + 1) + x;
                int n3 = y * (X_SEGMENTS + 1) + x;

                indices.emplace_back(n2);
                indices.emplace_back(n1);
                indices.emplace_back(n0);

                indices.emplace_back(n3);
                indices.emplace_back(n1);
                indices.emplace_back(n2);
            }
        }
        oddRow = !oddRow;
    }
    
    std::ofstream out("resources/models/sphere.obj", std::ios::out);
    for (int i = 0; i < positions.size(); i++)
    {
        out << "v " << positions[i].x << " " << positions[i].y << " " << positions[i].z << std::endl;
        out << "vt " << uv[i].x << " " << uv[i].y << std::endl;
        out << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << std::endl;
    }
	for (int i = 0; i < indices.size(); i+=3)
	{
        out << "f " << indices[i] + 1 << "/" << indices[i] + 1 << "/" << indices[i] + 1
            << " " << indices[i + 1] + 1 << "/" << indices[i + 1] + 1 << "/" << indices[i + 1] + 1
            << " " << indices[i + 2] + 1 << "/" << indices[i + 2] + 1 << "/" << indices[i + 2] + 1 << std::endl;
	}
    out.close();
}

RPGGame::RPGGame(GLuint width, GLuint height) :GameBase(width, height)
{

}

RPGGame::~RPGGame()
{

}

void RPGGame::Init()
{
    this->camera = new FPSCamera(glm::vec3(0, 1, 10), glm::vec3(0, 1, 0));

	//Direction Light
	this->scene->directionLight.direction = glm::normalize(glm::vec3(-1, -2, -1));
	this->scene->directionLight.color = glm::vec3(1, 1, 1);

	//Point Light
	PointLight pointLight;
	pointLight.position = glm::vec3(0, 5, 10);
	pointLight.color = glm::vec3(1, 1, 1);
	//this->scene->pointLights.emplace_back(pointLight);

	// lights
	// ------
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	for (int i = 0; i < 4; i++)
	{
		PointLight pointLight;
		pointLight.position = lightPositions[i];
		pointLight.color = lightColors[i];
		this->scene->pointLights.emplace_back(pointLight);
	}
    this->renderer->enablePointLightShadow = false;
	
	//Add Skybox
	this->scene->addSkyBox();
	
	//Add Shadow
	this->renderer->addShadowProcessor(this->Width, this->Height);

	Mesh* ground = ResourceManager::LoadMesh("resources/models/plane.obj", "ground");
	Material* groundMaterial = ResourceManager::LoadMaterial(Material::MaterialType::DIFFUSE, "ground");
	groundMaterial->isCastShadow = false;
	groundMaterial->isReceiveShadow = true;
	dynamic_cast<DiffuseMaterial*>(groundMaterial)->setupForwardShader(this->scene->directionLight, 
		pointLight, camera, glm::vec3(0.5, 0.5, 0.5));
	this->scene->addRenderObject(ground, groundMaterial, Transform(glm::vec3(10)), "ground");

	//Mesh* cat = ResourceManager::LoadMesh("resources/models/cat.obj", "cat");
	//Material* catMaterial = ResourceManager::LoadMaterial(Material::MaterialType::PBR, "cat");
	//catMaterial->isCastShadow = true;
	//catMaterial->isReceiveShadow = false;
	//dynamic_cast<PBRMaterial*>(catMaterial)->setupForwardShader(this->scene->pointLights, camera, 
	//	glm::vec3(0.5, 0, 0), 1, 0.1, 1);
	//this->scene->addRenderObject(cat, catMaterial, Transform(glm::vec3(0.01)), "cat");

    Mesh* sphere = ResourceManager::LoadMesh("resources/models/sphere.obj", "sphere");

    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    float metallic;
    float roughness;
    float ao = 1;

    for (int row = 0; row < nrRows; ++row)
    {
        metallic = (float)row / (float)nrRows;
        for (int col = 0; col < nrColumns; ++col)
        {
            // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
            // on direct lighting.
            roughness = glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f);

            Material* sphereMaterial = ResourceManager::LoadMaterial(Material::MaterialType::PBR, 
                "sphere" + std::to_string(row * nrColumns + col));
            sphereMaterial->isCastShadow = true;
            sphereMaterial->isReceiveShadow = false;
            dynamic_cast<PBRMaterial*>(sphereMaterial)->setupForwardShader(this->scene->pointLights, camera,
                glm::vec3(0.5, 0, 0), metallic, roughness, ao);
            this->scene->addRenderObject(sphere, sphereMaterial, Transform(glm::vec3(1), glm::vec3(
                (col - (nrColumns / 2)) * spacing,
                (row - (nrRows / 2)) * spacing+10,
                0.0f
            )), "sphere" + std::to_string(row * nrColumns + col));
        }
    }
}

void RPGGame::Update(GLfloat dt)
{

}

void RPGGame::ProcessInput(GLfloat dt)
{

}

void RPGGame::ProcessMouse(double xpos, double ypos)
{

}

void RPGGame::ProcessScroll(double xoffset, double yoffset)
{

}
