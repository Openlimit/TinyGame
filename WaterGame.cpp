#include "WaterGame.h"
#include <random>

#include "FPSCamera.h"

void createGrid(glm::vec2 topLeft, glm::vec2 downRight, float H, int width, int height,
    std::vector<glm::vec3> &vertices, std::vector<GLuint> &indices)
{
    int point_width = width + 1;
    int point_height = height + 1;
    vertices.resize(point_width * point_height);
    for (int y = 0; y <= height; ++y)
    {
        for (int x = 0; x <= width; ++x)
        {
            float u = (float)x / (float)width;
            float v = (float)y / (float)height;
            glm::vec2 point = glm::vec2(1 - u, 1 - v) * topLeft + glm::vec2(u, v) * downRight;
            vertices[y * point_width + x] = glm::vec3(point.x, H, point.y);
        }
    }

    bool oddRow = false;
    for (int y = 0; y < height; ++y)
    {
        if (!oddRow) // even rows
        {
            for (int x = 1; x <= width; ++x)
            {
                int n0 = y * point_width + x - 1;
                int n1 = (y + 1) * point_width + x - 1;
                int n2 = y * point_width + x;
                int n3 = (y + 1) * point_width + x;

                indices.emplace_back(n0);
                indices.emplace_back(n1);
                indices.emplace_back(n2);

                indices.emplace_back(n2);
                indices.emplace_back(n1);
                indices.emplace_back(n3);
            }
        }
        else
        {
            for (int x = width - 1; x >= 0; --x)
            {
                int n0 = (y + 1) * point_width + x + 1;
                int n1 = y * point_width + x + 1;
                int n2 = (y + 1) * point_width + x;
                int n3 = y * point_width + x;

                indices.emplace_back(n0);
                indices.emplace_back(n1);
                indices.emplace_back(n2);

                indices.emplace_back(n2);
                indices.emplace_back(n1);
                indices.emplace_back(n3);
            }
        }
        oddRow = !oddRow;
    }
}

WaterGame::WaterGame(GLuint width, GLuint height) : GameBase(width, height, Renderer::RendererType::DEFFERED)
{

}

WaterGame::~WaterGame()
{
    
}

void WaterGame::Init()
{
    this->time = 0;
	this->camera = new FPSCamera(glm::vec3(0, 5, 30), glm::vec3(0, 1, 0));

    /*std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    createGrid(glm::vec2(-10, -10), glm::vec2(10, 10), 0, 50, 50, vertices, indices);
    Mesh *water_plane = new Mesh(vertices, indices);
    Shader waterShader = ResourceManager::LoadShader("shaders/water.vs", "shaders/water.frag", nullptr, "water");
    waterShader.Use().SetMatrix4("model", glm::mat4());

    const int NUM = 4;
    GLfloat A[NUM];
    GLfloat W[NUM];
    GLfloat P[NUM];
    GLfloat D[NUM][2];
    std::default_random_engine e;
    e.seed(10);
    std::uniform_real_distribution<float> L_dis(0, 1);
    std::uniform_real_distribution<float> S_dis(0, 1);
    std::uniform_real_distribution<float> A_dis(0, 1);
    std::uniform_real_distribution<float> D_dis(0.1, 1);
    for (int i = 0; i < NUM; i++)
    {
        float L = L_dis(e);
        float S = S_dis(e);
        W[i] = 2. / L;
        P[i] = S * W[i];
        A[i] = A_dis(e);

        glm::vec2 dir = glm::normalize(glm::vec2(D_dis(e), D_dis(e)));
        D[i][0] = dir.x;
        D[i][1] = dir.y;
    }
    waterShader.SetFloatV("A", A, NUM);
    waterShader.SetFloatV("W", W, NUM);
    waterShader.SetFloatV("P", P, NUM);
    waterShader.Set2FloatV("D", D, NUM);

    waterShader.SetVector3f("light_pos", glm::vec3(-10, 10, -10));
    waterShader.SetVector3f("specular_color", glm::vec3(0.5));
    waterShader.SetVector3f("diffuse_color", glm::vec3(0, 180./255, 1));*/

    /*Mesh* ground = new Quad(-100, 100, -100, 100, -5, Quad::XZ);
    Shader groundShader = ResourceManager::LoadShader("shaders/mesh.vs", "shaders/mesh.frag", nullptr, "mesh");
    groundShader.Use();
    groundShader.SetMatrix4("model", glm::mat4());
    groundShader.SetVector3f("color", glm::vec3(0.5, 0.5, 0.5));*/

    //this->renderObjects.emplace_back(new RenderObject(water_plane, waterShader));
    //this->renderObjects.emplace_back(new RenderObject(ground, groundShader));

    this->postShader = ResourceManager::LoadShader("shaders/screen_quad.vs", "shaders/water_postprocess.frag", nullptr, "water_postprocess");
    this->postShader->Use();
    this->postShader->SetInteger("scene_color", 0);
    this->postShader->SetInteger("gPosition", 1);
    this->postShader->SetFloat("near_plane", 0.1);
    this->postShader->SetFloat("far_plane", 100);
    this->postShader->SetFloat("fog_start", 20);
    this->postShader->SetFloat("fog_end", 50);
    this->postShader->SetFloat("fog_density", 0.02);
    this->postShader->SetVector3f("fog_color", glm::vec3(0.3, 0.73, 0.63));

    this->postShader->SetVector3f("cam_pos", this->camera->Position);
    this->postShader->SetVector3f("light_color", glm::vec3(10, 10, 10));
    this->postShader->SetVector3f("light_pos", glm::vec3(10, 10, 10));
    //this->postShader.SetVector3f("extinction", glm::vec3(1.2, 0.31, 0.46));
    this->postShader->SetVector3f("extinction", glm::vec3(0.46, 0.31, 1.2));
    this->postShader->SetInteger("step_num", 4);
    this->postShader->SetFloat("distance_ratio", 0.05);

    this->renderer->addPostProcessor(this->postShader, Width, Height);
}

void WaterGame::Update(GLfloat dt)
{
    this->postShader->Use()->SetVector3f("cam_pos", this->camera->Position);

    /*this->renderObjects[0]->forwardShader.Use();
    this->renderObjects[0]->forwardShader.SetMatrix4("view", view);
    this->renderObjects[0]->forwardShader.SetMatrix4("projection", projection);

    this->time += dt;
    this->renderObjects[0]->forwardShader.SetFloat("time", this->time);
    this->renderObjects[0]->forwardShader.SetVector3f("view_pos", this->camera.Position);

    this->renderObjects[1]->forwardShader.Use();
    this->renderObjects[1]->forwardShader.SetMatrix4("view", view);
    this->renderObjects[1]->forwardShader.SetMatrix4("projection", projection);*/
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

