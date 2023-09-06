#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glad/glad.h> 
#include <stb/stb_image.h>

#include "Transform.h"

LModel::LModel(char const* Path)
{
	LoadModel(Path);
}

void LModel::Draw(const LMaterial& Material, const LTransform& Transform) const
{
	for (const LMesh& Mesh : Meshes)
	{
		Mesh.Draw(Material, Transform);
	}
}

void LModel::LoadModel(char const* ModelPath)
{
	std::string Path{ ModelPath };

	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << "\n";
		return;
	}

	Directory = Path.substr(0, Path.find_last_of('/'));
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
	std::vector<LTexture> Textures;

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

	// Process Material
	// Do we want to set materials up independantly?
	/*if (Mesh->mMaterialIndex >= 0)
	{
		aiMaterial* Material = Scene->mMaterials[Mesh->mMaterialIndex];
		std::vector<LTexture> DiffuseMaps = LoadMaterialTextures(Material, aiTextureType_DIFFUSE, "DiffuseTexture");
		Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

		std::vector<LTexture> SpecularMaps = LoadMaterialTextures(Material, aiTextureType_SPECULAR, "SpecularTexture");
		Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
	}*/

	return LMesh(Vertices, Indices);
}

//std::vector<LTexture> LModel::LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, char const* TypeName)
//{
//	std::vector<LTexture> Textures;
//	for (unsigned int i = 0; i < Mat->GetTextureCount(Type); i++)
//	{
//		aiString String;
//		Mat->GetTexture(Type, i, &String);
//		
//		const unsigned int TextureId = TextureFromFile(String.C_Str(), Directory);
//		ETextureType TextureType;
//		switch (Type)
//		{
//		case aiTextureType::aiTextureType_DIFFUSE:
//			TextureType = ETextureType::Diffuse;
//			break;
//		case aiTextureType::aiTextureType_SPECULAR:
//			TextureType = ETextureType::Specular;
//			break;
//		}
//
//		LTexture Texture(TextureId, TextureType, String.C_Str());
//
//		Textures.push_back(Texture);
//	}
//
//	return Textures;
//}
//
//unsigned int TextureFromFile(const char* Path, const std::string& Directory, bool Gamma = false)
//{
//	std::string filename = std::string(Path);
//	filename = Directory + '/' + filename;
//
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << Path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}
