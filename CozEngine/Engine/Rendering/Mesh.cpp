#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

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

	{{-0.5f,  0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f}, { 1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f}, { 1.0f, 1.0f}},

	{{0.5f,  0.5f,  0.5f }, {1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f}},
	{{0.5f,  0.5f, -0.5f }, {1.0f,  0.0f,  0.0f}, { 1.0f, 1.0f}},
	{{0.5f, -0.5f, -0.5f }, {1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f }, {1.0f,  0.0f,  0.0f}, { 1.0f, 0.0f}},
	{{0.5f, -0.5f,  0.5f }, {1.0f,  0.0f,  0.0f}, { 0.0f, 0.0f}},
	{{0.5f,  0.5f,  0.5f }, {1.0f,  0.0f,  0.0f}, { 0.0f, 1.0f}},

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
	Vertices = TestVertices;

	SetupMesh();
}

LMesh::LMesh(const std::vector<Vertex>& i_Vertices, const std::vector<unsigned int> i_Indices)
{
	 Vertices = i_Vertices;
	 Indices = i_Indices;

	 SetupMesh();
}

void LMesh::Draw(const LShader& Shader, const glm::mat4& Transform) const
{
	Shader.SetMat4("Model", Transform);

	glm::mat3 NormalMatrix = glm::transpose(glm::inverse(Transform));
	Shader.SetMat3("NormalMatrix", NormalMatrix);

	glBindVertexArray(VAO);
	if (!Indices.empty())
	{
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
	}
	glBindVertexArray(0);
}

void LMesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	assert(Vertices.size() > 0);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	if (Indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
