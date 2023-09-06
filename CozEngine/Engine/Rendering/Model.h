#pragma once

#include <assimp/postprocess.h>
#include <iostream>
#include <vector>

#include "Mesh.h"
#include "Texture.h"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

struct LMaterial;
// class LTexture;

class LModel
{
public:
	LModel(char const* Path);
	void Draw(const LMaterial& Material, const LTransform& Transform) const;

private:
	std::vector<LMesh> Meshes;
	std::string Directory;

	void LoadModel(char const* ModelPath);
	void ProcessNode(aiNode* Node, const aiScene* Scene);
	LMesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
	// std::vector<LTexture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, char const* TypeName);
};

