#include "WaterGame.h"
#include <random>

static std::vector<glm::vec3> createGrid(glm::vec2 topLeft, glm::vec2 downRight, float H, int width, int height)
{
    int point_width = width + 1;
    int point_height = height + 1;
    glm::vec3* points = new glm::vec3[point_width * point_height];
    for (int y = 0; y <= height; ++y)
    {
        for (int x = 0; x <= width; ++x)
        {
            float u = (float)x / (float)width;
            float v = (float)y / (float)height;
            glm::vec2 point = glm::vec2(1 - u, 1 - v) * topLeft + glm::vec2(u, v) * downRight;
            points[y * point_width + x] = glm::vec3(point.x, H, point.y);
        }
    }

    std::vector<glm::vec3> grid_points;
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

                grid_points.emplace_back(points[n0]);
                grid_points.emplace_back(points[n1]);
                grid_points.emplace_back(points[n2]);

                grid_points.emplace_back(points[n2]);
                grid_points.emplace_back(points[n1]);
                grid_points.emplace_back(points[n3]);
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

                grid_points.emplace_back(points[n0]);
                grid_points.emplace_back(points[n1]);
                grid_points.emplace_back(points[n2]);

                grid_points.emplace_back(points[n2]);
                grid_points.emplace_back(points[n1]);
                grid_points.emplace_back(points[n3]);
            }
        }
        oddRow = !oddRow;
    }

    delete[] points;

    return grid_points;
}

WaterGame::WaterGame(GLuint width, GLuint height) : Width(width), Height(height)
{

}

WaterGame::~WaterGame()
{

}

void WaterGame::Init()
{
	this->camera = Camera(glm::vec3(0, 20, 20), glm::vec3(0, 1, 0));

    std::vector<glm::vec3> grid = createGrid(glm::vec2(-10, -10), glm::vec2(10, 10), 0, 50, 50);
    this->plane = new Mesh(grid);

    this->waterShader = ResourceManager::LoadShader("shaders/water.vs", "shaders/water.frag", nullptr, "water");
    this->waterShader.Use().SetMatrix4("model", glm::mat4());

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
    this->waterShader.SetFloatV("A", A, NUM);
    this->waterShader.SetFloatV("W", W, NUM);
    this->waterShader.SetFloatV("P", P, NUM);
    this->waterShader.Set2FloatV("D", D, NUM);

    this->waterShader.SetVector3f("light_pos", glm::vec3(-10, 10, -10));
    this->waterShader.SetVector3f("specular_color", glm::vec3(0.5));
    this->waterShader.SetVector3f("diffuse_color", glm::vec3(0, 180./255, 1));

    this->time = 0;
}

void WaterGame::ProcessInput(GLfloat dt)
{
    if (this->Keys[GLFW_KEY_W])
        this->camera.ProcessKeyboard(FORWARD, dt);
    if (this->Keys[GLFW_KEY_S])
        this->camera.ProcessKeyboard(BACKWARD, dt);
    if (this->Keys[GLFW_KEY_A])
        this->camera.ProcessKeyboard(LEFT, dt);
    if (this->Keys[GLFW_KEY_D])
        this->camera.ProcessKeyboard(RIGHT, dt);
}

float lastX = 0;
float lastY = 0;
bool firstMouse = true;
void WaterGame::ProcessMouse(double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    this->camera.ProcessMouseMovement(xoffset, yoffset);
}

void WaterGame::ProcessScroll(double xoffset, double yoffset)
{
    this->camera.ProcessMouseScroll(yoffset);
}

void WaterGame::Update(GLfloat dt)
{
    glm::mat4 view = this->camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(this->camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
    this->waterShader.Use();
    this->waterShader.SetMatrix4("view", view);
    this->waterShader.SetMatrix4("projection", projection);
    
    this->time += dt;
    this->waterShader.SetFloat("time", this->time);

    this->waterShader.SetVector3f("view_pos", this->camera.Position);
}

void WaterGame::Render()
{
    this->renderer.Draw(*(this->plane), this->waterShader);
}