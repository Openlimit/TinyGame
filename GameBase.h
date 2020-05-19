#pragma once
#include "Scene.h"
#include "Camera.h"
#include "ForwardRenderer.h"
#include "DeferredRenderer.h"
#include "ResourceManager.h"

class GameBase {
public:
    GLboolean Keys[1024];
    GLuint Width, Height;
    Camera* camera;
    Renderer* renderer;
    Scene *scene;
    bool isFirstFrame;


    GameBase(GLuint width, GLuint height, Renderer::RendererType type = Renderer::RendererType::FORWARD) :Width(width), Height(height), isFirstFrame(true)
    {
        if (type == Renderer::RendererType::FORWARD)
        {
            this->renderer = new ForwardRenderer(Width, Height);
        }
        else if (type == Renderer::RendererType::DEFFERED)
        {
            this->renderer = new DeferredRenderer(Width, Height);
        }

        this->scene = new Scene();
    }

    ~GameBase()
    {
        if (this->renderer != nullptr)
            delete this->renderer;
        if (this->scene != nullptr)
            delete this->scene;
    }

    void Render()
    {
        if (isFirstFrame)
        {
            this->renderer->Init(this->scene, camera);
            isFirstFrame = false;
        }

        this->renderer->Draw(this->scene, camera);
    }

    void GameProcessInput(GLfloat dt)
    {
        this->camera->ProcessInput(this->Keys, dt);
        this->ProcessInput(dt);
    }

    void GameProcessMouse(double xpos, double ypos)
    {
        this->camera->ProcessMouse(xpos, ypos);
        this->ProcessMouse(xpos, ypos);
    }

    void GameProcessScroll(double xoffset, double yoffset)
    {
        this->camera->ProcessScroll(xoffset, yoffset);
        this->ProcessScroll(xoffset, yoffset);
    }

    void GameUpdate(GLfloat dt)
    {
        auto view = this->camera->GetViewMatrix();
        auto projection = this->camera->GetProjectionMatrix((float)Width / (float)Height);
        
        for (auto iter : ResourceManager::Shaders)
        {
            if (iter.second->auto_update_VP)
            {
                iter.second->Use();
                iter.second->SetMatrix4("view", view);
                iter.second->SetMatrix4("projection", projection);
            }
        }

        this->Update(dt);
    }

    virtual void Init() = 0;

protected:
    virtual void Update(GLfloat dt) = 0;

    virtual void ProcessInput(GLfloat dt) = 0;
    virtual void ProcessMouse(double xpos, double ypos) = 0;
    virtual void ProcessScroll(double xoffset, double yoffset) = 0;
};