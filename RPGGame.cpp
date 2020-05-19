#include "RPGGame.h"
#include "FPSCamera.h"
#include <fstream>
#include "GameObject.h"
#include "FollowCamera.h"

RPGGame::RPGGame(GLuint width, GLuint height) :GameBase(width, height)
{

}

RPGGame::~RPGGame()
{

}

void RPGGame::Init()
{
    camera = new FollowCamera(glm::vec3(0, 15, 15), glm::vec3(0));
    
    scene->load("resources/scenes/rpg.scene");

	//Add Skybox
	scene->addSkyBox();
	
    renderer->enablePointLightShadow = false;

	//Add Shadow
	renderer->addShadowProcessor(this->Width, this->Height);

	Material* groundMaterial = ResourceManager::GetMaterial("ground");
	dynamic_cast<DiffuseMaterial*>(groundMaterial)->setupForwardShader(scene->directionLight, 
		scene->pointLights[0], camera, glm::vec3(0.5, 0.5, 0.5));

    float metallic = 0.2;
    float roughness = 0.2;
    float ao = 1;
    Material* sphereMaterial = ResourceManager::GetMaterial("sphere");
    dynamic_cast<PBRMaterial*>(sphereMaterial)->setupForwardShader(scene->pointLights, camera,
        glm::vec3(0.5, 0, 0), metallic, roughness, ao);

    player = new GameObject(scene->renderObjects["sphere"]);
    player->move_speed = 10;
}

void RPGGame::Update(GLfloat dt)
{

}

void RPGGame::ProcessInput(GLfloat dt)
{
    if (Keys[GLFW_KEY_W])
        player->move(GameObject::MoveDirection::FORWARD, dt);
    if (Keys[GLFW_KEY_S])
        player->move(GameObject::MoveDirection::BACKWARD, dt);
    if (Keys[GLFW_KEY_A])
        player->move(GameObject::MoveDirection::LEFT, dt);
    if (Keys[GLFW_KEY_D])
        player->move(GameObject::MoveDirection::RIGHT, dt);
}

void RPGGame::ProcessMouse(double xpos, double ypos)
{

}

void RPGGame::ProcessScroll(double xoffset, double yoffset)
{

}
