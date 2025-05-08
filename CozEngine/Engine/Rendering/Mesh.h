#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "json.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

inline void to_json(nlohmann::json& J, const Vertex& V)
{
	J = {
		{"PosX", V.Position.x},
		{"PosY", V.Position.y},
		{"PosZ", V.Position.z},

		{"NormalX", V.Normal.x},
		{"NormalY", V.Normal.y},
		{"NormalZ", V.Normal.z},

		{"TexX", V.TexCoords.x},
		{"TexY", V.TexCoords.y}
	};
}

inline void from_json(const nlohmann::json& J, Vertex& V)
{
	J.at("PosX").get_to(V.Position.x);
	J.at("PosY").get_to(V.Position.y);
	J.at("PosZ").get_to(V.Position.z);

	J.at("NormalX").get_to(V.Normal.x);
	J.at("NormalY").get_to(V.Normal.y);
	J.at("NormalZ").get_to(V.Normal.z);

	J.at("TexX").get_to(V.TexCoords.x);
	J.at("TexY").get_to(V.TexCoords.y);
}

class LShader;

class LMesh
{
public:
	LMesh() {}
	LMesh(const std::vector<Vertex>& i_Vertices, const std::vector<unsigned int> i_Indices);
	LMesh(std::vector<Vertex>&& i_Vertices, std::vector<unsigned int>&& i_Indices);

	LMesh(LMesh&& Other);

	~LMesh();

	void Draw(const LShader& Mat, const glm::mat4& Transform) const;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LMesh, Vertices, Indices)

private:
	void SetupMesh();

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};

