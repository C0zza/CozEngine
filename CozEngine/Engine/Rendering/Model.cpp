#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glad/glad.h> 
#include <stb/stb_image.h>

#include "Misc/Logging.h"
#include "Transform.h"

void LModel::Load()
{
	if (ObjFile.empty())
	{
		Log(LLogLevel::INFO, "LModel::Load - Empty LModel object loaded. Adding default cube.");
		Meshes.push_back(LMesh());
		return;
	}

	LoadModel(ObjFile.c_str());
}

void LModel::Draw(const LShader& Shader, const glm::mat4& Transform) const
{
	for (const LMesh& Mesh : Meshes)
	{
		Mesh.Draw(Shader, Transform);
	}
}

void LModel::LoadModel(const std::string& ModelPath)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		Log(LLogLevel::ERROR, "ERROR::ASSIMP::" + std::string(Importer.GetErrorString()));
		return;
	}

	ProcessNode(Scene->mRootNode, Scene);
}

void LModel::ProcessNode(aiNode* Node, const aiScene* Scene)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(Mesh, Scene));
	}

	// Do the same for each of it's children
	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i], Scene);
	}
}

LMesh LModel::ProcessMesh(aiMesh* Mesh, const aiScene* Scene)
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	// Process Vertices
	for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
	{
		Vertex Vert;
		
		glm::vec3 Vec3;
		Vec3.x = Mesh->mVertices[i].x;
		Vec3.y = Mesh->mVertices[i].y;
		Vec3.z = Mesh->mVertices[i].z;
		Vert.Position = Vec3;

		Vec3.x = Mesh->mNormals[i].x;
		Vec3.y = Mesh->mNormals[i].y;
		Vec3.z = Mesh->mNormals[i].z;
		Vert.Normal = Vec3;

		// Assimp allows vertices to have 8 sets of texture coords per vertex. We only want the first
		if (Mesh->mTextureCoords[0])
		{
			glm::vec2 Vec2;
			Vec2.x = Mesh->mTextureCoords[0][i].x;
			Vec2.y = Mesh->mTextureCoords[0][i].y;
			Vert.TexCoords = Vec2;
		}
		else
		{
			Vert.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		Vertices.push_back(Vert);
	}

	// Process Indices
	for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
	{
		const aiFace& Face = Mesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}

	return LMesh(Vertices, Indices);
}
