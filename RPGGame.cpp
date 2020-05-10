#include "RPGGame.h"
#include "FPSCamera.h"

RPGGame::RPGGame(GLuint width, GLuint height) :GameBase(width, height)
{

}

RPGGame::~RPGGame()
{

}

void RPGGame::Init()
{
	this->camera = new FPSCamera(glm::vec3(0, 1, 10), glm::vec3(0, 1, 0));
	this->scene->directionLight.direction = glm::normalize(glm::vec3(-1, -2, -1));
	this->scene->directionLight.color = glm::vec3(1, 1, 1);
	
	this->renderer->addShadowProcessor(this->Width, this->Height);

	Mesh* ground = ResourceManager::LoadMesh("resources/models/plane.obj", "ground");
	Material* groundMaterial = ResourceManager::LoadMaterial(Material::MaterialType::DIFFUSE, "ground");
	groundMaterial->isCastShadow = true;
	groundMaterial->isReceiveShadow = true;
	dynamic_cast<DiffuseMaterial*>(groundMaterial)->setupForwardShader(this->scene->directionLight, 
		camera, glm::vec3(0.5, 0.5, 0.5));
	this->scene->addRenderObject(ground, groundMaterial, Transform(glm::vec3(10)), "ground");

	Mesh* cat = ResourceManager::LoadMesh("resources/models/cat.obj", "cat");
	Material* catMaterial = ResourceManager::LoadMaterial(Material::MaterialType::DIFFUSE, "cat");
	catMaterial->isCastShadow = true;
	catMaterial->isReceiveShadow = true;
	dynamic_cast<DiffuseMaterial*>(catMaterial)->setupForwardShader(this->scene->directionLight, 
		camera, glm::vec3(0.3, 0.5, 0.7));
	this->scene->addRenderObject(cat, catMaterial, Transform(glm::vec3(0.01)), "cat");
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
