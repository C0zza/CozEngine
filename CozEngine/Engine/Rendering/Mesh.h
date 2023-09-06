#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Transform.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct LMaterial;

class LMesh
{
public:
	LMesh();
	LMesh(const std::vector<Vertex>& i_Vertices, const std::vector<unsigned int> i_Indices);

	void Draw(const LMaterial& Mat, const LTransform& Transform) const;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
private:
	void SetupMesh();

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};

