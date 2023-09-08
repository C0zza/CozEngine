#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "Material.h"
#include "Shader.h"
#include "Texture.h"

// Dummy cube data
std::vector<Vertex> TestVertices = 
{
	{{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, { 1.0f, 0.0f}},
	{{0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, { 1.0f, 1.0f}},
	{{0.5f,  0.5f, -0.5f }, {0.0f,  0.0f, -1.0f}, { 1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f }, {0.0f,  0.0f, -1.0f}, { 0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f }, {0.0f,  0.0f, -1.0f}, { 0.0f, 0.0f}},

	{{-0.5f, -0.5f,  0.5f }, {0.0f,  0.0f,  1.0f}, { 0.0f, 0.0f}},
	{{0.5f, -0.5f,  0.5f }, {0.0f,  0.0f,  1.0f}, { 1.0f, 0.0f}},
	{{0.5f,  0.5f,  0.5f }, {0.0f,  0.0f,  1.0f}, { 1.0f, 1.0f}},
	{{0.5f,  0.5f,  0.5f }, {0.0f,  0.0f,  1.0f}, { 1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f }, {0.0f,  0.0f,  1.0f}, { 0.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f }, {0.0f,  0.0f,  1.0f}, { 0.0f, 0.0f}},

	{{-0.5f,  0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f}, { 1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f}},

	{{0.5f,  0.5f,  0.5f }, {1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f}},
	{{0.5f,  0.5f, -0.5f }, {1.0f,  0.0f,  0.0f}, { 1.0f, 1.0f}},
	{{0.5f, -0.5f, -0.5f }, {1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f}},
	{{0.5f, -0.5f, -0.5f }, {1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f}},
	{{0.5f, -0.5f,  0.5f }, {1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f}},
	{{0.5f,  0.5f,  0.5f }, {1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f}},

	{{-0.5f, -0.5f, -0.5f }, {0.0f, -1.0f,  0.0f}, { 0.0f, 1.0f}},
	{{0.5f, -0.5f, -0.5f }, {0.0f, -1.0f,  0.0f}, { 1.0f, 1.0f}},
	{{0.5f, -0.5f,  0.5f }, {0.0f, -1.0f,  0.0f}, { 1.0f, 0.0f}},
	{{0.5f, -0.5f,  0.5f }, {0.0f, -1.0f,  0.0f}, { 1.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.5f }, {0.0f, -1.0f,  0.0f}, { 0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f }, {0.0f, -1.0f,  0.0f}, { 0.0f, 1.0f}},

	{{-0.5f,  0.5f, -0.5f }, {0.0f,  1.0f,  0.0f}, { 0.0f, 1.0f}},
	{{0.5f,  0.5f, -0.5f }, {0.0f,  1.0f,  0.0f}, { 1.0f, 1.0f}},
	{{0.5f,  0.5f,  0.5f }, {0.0f,  1.0f,  0.0f}, { 1.0f, 0.0f}},
	{{0.5f,  0.5f,  0.5f }, {0.0f,  1.0f,  0.0f}, { 1.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f }, {0.0f,  1.0f,  0.0f}, { 0.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f }, {0.0f,  1.0f,  0.0f}, { 0.0f, 1.0f}}
};

LMesh::LMesh(/*const std::vector<Vertex>& i_Vertices , const std::vector<unsigned int> i_Indices*/)
{
	// Vertices = i_Vertices;
	// Indices = i_Indices;

	Vertices = TestVertices;

	SetupMesh();
}

LMesh::LMesh(const std::vector<Vertex>& i_Vertices, const std::vector<unsigned int> i_Indices)
{
	 Vertices = i_Vertices;
	 Indices = i_Indices;

	 SetupMesh();
}

void LMesh::Draw(const LMaterial& Mat, const LTransform& Transform) const
{
	std::string TextureString;
	unsigned int DiffuseCount = 0;
	unsigned int SpecularCount = 0;

	// Temp save on setting shader each mesh draw
	static LShader* CurrentShader = nullptr;
	if (!CurrentShader)
	{
		CurrentShader = Mat.Shader.get();
		assert(CurrentShader);
		CurrentShader->Use();
	}
	else
	{
		assert(CurrentShader);
	}

	glm::mat4 Transformation = glm::mat4(1.f);
	Transformation = glm::translate(Transformation, Transform.GetPosition());
	Transformation = glm::scale(Transformation, Transform.GetScale());
	Transformation = glm::rotate(Transformation, Transform.GetRotation().x, glm::vec3(1.0, 0.f, 0.f));
	Transformation = glm::rotate(Transformation, Transform.GetRotation().y, glm::vec3(0.f, 1.0f, 0.f));
	Transformation = glm::rotate(Transformation, Transform.GetRotation().z, glm::vec3(0.f, 0.f, 1.0f));
	CurrentShader->SetMat4("Model", Transformation);

	glm::mat3 NormalMatrix = glm::transpose(glm::inverse(Transformation));
	CurrentShader->SetMat3("NormalMatrix", NormalMatrix);

	for (unsigned int i = 0; i < Mat.Textures.size(); i++)
	{
		LTexture* Tex = Mat.Textures[i].get();
		assert(Tex);

		switch (Tex->GetTextureType())
		{
		case ETextureType::Diffuse:
			TextureString = "DiffuseTexture" + std::to_string(++DiffuseCount);
			break;
		case ETextureType::Specular:
			TextureString = "SpecularTexture" + std::to_string(++SpecularCount);
			break;
		}

		CurrentShader->SetInt(TextureString.c_str(), i);
		Tex->Use(i);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void LMesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
