#include "utils.h"
#include "OpenGL_Common.h"
#include <vector>
#include <iostream>
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
    for (int i = 0; i < indices.size(); i += 3)
    {
        out << "f " << indices[i] + 1 << "/" << indices[i] + 1 << "/" << indices[i] + 1
            << " " << indices[i + 1] + 1 << "/" << indices[i + 1] + 1 << "/" << indices[i + 1] + 1
            << " " << indices[i + 2] + 1 << "/" << indices[i + 2] + 1 << "/" << indices[i + 2] + 1 << std::endl;
    }
    out.close();
}