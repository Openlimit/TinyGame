#pragma once
#include "Material.h"
#include "ResourceManager.h"
#include <random>

class WaterMaterial :public Material
{
public:
    float* time;

	WaterMaterial(glm::vec3 diffuse_color, float *time) :Material(),time(time)
	{
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

		forwardShader = ResourceManager::GetShader("water");
		if (forwardShader == nullptr)
			forwardShader = ResourceManager::LoadShader("shaders/water.vs", "shaders/water.frag", nullptr, "water");

        forwardShader->auto_update_VP = true;
        forwardShader->Use();
        forwardShader->SetFloatV("A", A, NUM);
        forwardShader->SetFloatV("W", W, NUM);
        forwardShader->SetFloatV("P", P, NUM);
        forwardShader->Set2FloatV("D", D, NUM);

        forwardShader->SetVector3f("diffuse_color", diffuse_color);
	}

    void updateForwardShader() override
    {
        forwardShader->SetFloat("time", *time);
    }
};