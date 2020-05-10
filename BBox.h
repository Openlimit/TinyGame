#pragma once
#include <climits>

#include "OpenGL_Common.h"
#include "Mesh.h"

class BBox
{
public:
	glm::vec3 min_pos;
	glm::vec3 max_pos;

	std::vector<glm::vec3> getBPosition()
	{
		std::vector<glm::vec3> positions(8);
		positions[0] = min_pos;
		positions[1] = glm::vec3(max_pos.x, min_pos.y, min_pos.z);
		positions[2] = glm::vec3(max_pos.x, min_pos.y, max_pos.z);
		positions[3] = glm::vec3(min_pos.x, min_pos.y, max_pos.z);
		positions[4] = glm::vec3(min_pos.x, max_pos.y, min_pos.z);
		positions[5] = glm::vec3(max_pos.x, max_pos.y, min_pos.z);
		positions[6] = max_pos;
		positions[7] = glm::vec3(min_pos.x, max_pos.y, max_pos.z);
		return positions;
	}

	static BBox merge(BBox a, BBox b)
	{
		BBox c;
		c.max_pos = glm::max(a.max_pos, b.max_pos);
		c.min_pos = glm::min(a.min_pos, b.min_pos);
		return c;
	}

	static BBox getBox(Mesh *mesh)
	{
		BBox box;
		box.min_pos = glm::vec3(INT_MAX);
		box.max_pos = glm::vec3(INT_MIN);
		for (auto vertex : mesh->vertices)
		{
			box.min_pos = glm::min(box.min_pos, vertex.Position);
			box.max_pos = glm::max(box.max_pos, vertex.Position);
		}
		box.min_pos -= glm::vec3(0.01);
		box.max_pos += glm::vec3(0.01);
		return box;
	}

	static BBox getBox(std::vector<glm::vec3> &positions)
	{
		BBox box;
		box.min_pos = glm::vec3(INT_MAX);
		box.max_pos = glm::vec3(INT_MIN);
		for (auto pos : positions)
		{
			box.min_pos = glm::min(box.min_pos, pos);
			box.max_pos = glm::max(box.max_pos, pos);
		}
		box.min_pos -= glm::vec3(0.01);
		box.max_pos += glm::vec3(0.01);
		return box;
	}
};