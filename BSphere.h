#pragma once
#include "OpenGL_Common.h"
#include "Mesh.h"

class BSphere
{
public:
	glm::vec3 center;
	float radius;

	static BSphere merge(BSphere a, BSphere b)
	{

		float dist = glm::length(a.center - b.center);
		if (dist + b.radius <= a.radius)
		{
			return a;
		}
		else if (dist + a.radius <= b.radius)
		{
			return b;
		}
		else
		{
			BSphere c;
			glm::vec3 ab = glm::normalize(b.center - a.center);
			glm::vec3 A = a.center - a.radius * ab;
			glm::vec3 B = b.center + b.radius * ab;
			c.center = (A + B) / 2.f;
			c.radius = glm::length(A - c.center);
			return c;
		}
	}

	static BSphere getBSphere(Mesh* mesh)
	{
		BSphere s;
		s.center = mesh->vertices[0].Position;
		s.radius = 0;
		for (auto vertex : mesh->vertices)
		{
			float dis = glm::length(s.center - vertex.Position);
			if (dis > s.radius)
			{
				glm::vec3 cp = glm::normalize(vertex.Position - s.center);
				glm::vec3 g = s.center - s.radius * cp;
				s.center = (g + vertex.Position) / 2.f;
				s.radius = glm::length(g - s.center);
			}
		}
		return s;
	}

	static BSphere getBSphere(std::vector<glm::vec3>& positions)
	{
		BSphere s;
		s.center = positions[0];
		s.radius = 0;
		for (auto pos : positions)
		{
			float dis = glm::length(s.center - pos);
			if (dis > s.radius)
			{
				glm::vec3 cp = glm::normalize(pos - s.center);
				glm::vec3 g = s.center - s.radius * cp;
				s.center = (g + pos) / 2.f;
				s.radius = glm::length(g - s.center);
			}
		}
		return s;
	}
};