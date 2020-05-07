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
	this->camera = new FPSCamera(glm::vec3(0, 10, 50), glm::vec3(0, 1, 0));

	Mesh* ground = ResourceManager::LoadMesh("resources/models/plane.obj", "ground");
	Shader* groundShader = ResourceManager::LoadShader("shaders/mesh.vs", "shaders/mesh.frag", nullptr, "ground");
	groundShader->auto_update_VP = true;
	groundShader->Use();
	groundShader->SetMatrix4("model", glm::scale(glm::mat4(), glm::vec3(100, 100, 100)));
	groundShader->SetVector3f("color", glm::vec3(0.5, 0.5, 0.5));
	this->scene->addRenderObject(ground, groundShader, "ground");

	Mesh* cat = ResourceManager::LoadMesh("resources/models/cat.obj", "cat");
	Shader* catShader = ResourceManager::LoadShader("shaders/mesh.vs", "shaders/mesh.frag", nullptr, "cat");
	catShader->auto_update_VP = true;
	catShader->Use();
	catShader->SetMatrix4("model", glm::scale(glm::mat4(), glm::vec3(0.01, 0.01, 0.01)));
	catShader->SetVector3f("color", glm::vec3(0.5, 0, 0.5));
	this->scene->addRenderObject(cat, catShader, "cat");

	Mesh* deer = ResourceManager::LoadMesh("resources/models/deer.obj", "deer");
	Shader* deerShader = ResourceManager::LoadShader("shaders/mesh.vs", "shaders/mesh.frag", nullptr, "deer");
	deerShader->auto_update_VP = true;
	deerShader->Use();
	deerShader->SetMatrix4("model", glm::scale(glm::mat4(), glm::vec3(0.01, 0.01, 0.01)));
	deerShader->SetVector3f("color", glm::vec3(0, 0.7, 0.7));
	this->scene->addRenderObject(deer, deerShader, "deer");
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
