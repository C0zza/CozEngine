#pragma once

#include <assimp/postprocess.h>
#include <vector>

#include "Mesh.h"
#include "Reflection/Reflection.h"
#include "ResourceManagement/Resource.h"
#include "ResourceManagement/ResourceHandleHelper.h"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

class LShader;

REFL_CLASS()
class LModel : public LResource
{
	REFL_GENERATED_BODY(LModel)
public:
	virtual void Load() override;
	virtual void Unload() override {}
	
	void Draw(const LShader& Shader, const glm::mat4& Transform) const;

private:
	std::vector<LMesh> Meshes;

	REFL_PROP(Visible)
	std::string ObjFile;

	void LoadModel(const std::string& ModelPath);
	void ProcessNode(aiNode* Node, const aiScene* Scene);
	LMesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
};

