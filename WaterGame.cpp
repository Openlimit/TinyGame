#include "WaterGame.h"
#include "FPSCamera.h"
#include "WaterMaterial.h"


WaterGame::WaterGame(GLuint width, GLuint height) : GameBase(width, height)
{

}

WaterGame::~WaterGame()
{
    
}

void WaterGame::Init()
{
    this->time = 0;
	this->camera = new FPSCamera(glm::vec3(0, 5, 30), glm::vec3(0, 1, 0));

    scene->load("resources/scenes/water.scene");
    scene->addSkyBox();

    auto waterMaterial = new WaterMaterial(glm::vec3(0.5), &time);
    ResourceManager::ReplaceMaterial("water", waterMaterial);
    scene->addMaterial("water_plane", waterMaterial);

    //this->postShader = ResourceManager::LoadShader("shaders/screen_quad.vs", "shaders/water_postprocess.frag", nullptr, "water_postprocess");
    //this->postShader->Use();
    //this->postShader->SetInteger("scene_color", 0);
    //this->postShader->SetInteger("gPosition", 1);
    //this->postShader->SetFloat("near_plane", 0.1);
    //this->postShader->SetFloat("far_plane", 100);
    //this->postShader->SetFloat("fog_start", 20);
    //this->postShader->SetFloat("fog_end", 50);
    //this->postShader->SetFloat("fog_density", 0.02);
    //this->postShader->SetVector3f("fog_color", glm::vec3(0.3, 0.73, 0.63));

    //this->postShader->SetVector3f("cam_pos", this->camera->Position);
    //this->postShader->SetVector3f("light_color", glm::vec3(10, 10, 10));
    //this->postShader->SetVector3f("light_pos", glm::vec3(10, 10, 10));
    ////this->postShader.SetVector3f("extinction", glm::vec3(1.2, 0.31, 0.46));
    //this->postShader->SetVector3f("extinction", glm::vec3(0.46, 0.31, 1.2));
    //this->postShader->SetInteger("step_num", 4);
    //this->postShader->SetFloat("distance_ratio", 0.05);

    //this->renderer->addPostProcessor(this->postShader, Width, Height);
}

void WaterGame::Update(GLfloat dt)
{
    //this->postShader->Use()->SetVector3f("cam_pos", this->camera->Position);

    this->time += dt;

}

void WaterGame::ProcessInput(GLfloat dt)
{
    
}

void WaterGame::ProcessMouse(double xpos, double ypos)
{
    
}

void WaterGame::ProcessScroll(double xoffset, double yoffset)
{
    
}

