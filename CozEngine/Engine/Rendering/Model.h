#pragma once

#include <assimp/postprocess.h>
#include <vector>

#include "Mesh.h"
#include "ResourceManagement/Resource.h"
#include "ResourceManagement/ResourceHandleHelper.h"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

class LShader;

class LModel : public LResource
{
public:
	virtual void Load() override;
	virtual void Unload() override {}
	
	void Draw(const LShader& Shader, const glm::mat4& Transform) const;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LModel, ObjFile)

private:
	std::vector<LMesh> Meshes;
	std::string ObjFile;

	void LoadModel(const std::string& ModelPath);
	void ProcessNode(aiNode* Node, const aiScene* Scene);
	LMesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
};

inline void to_json(nlohmann::json& J, const LResourceHandle<LModel>& ResourceHandle)
{
	LResourceHandleHelper::ToJsonHelper(J, ResourceHandle);
}

inline void from_json(const nlohmann::json& J, LResourceHandle<LModel>& ResourceHandle)
{
	LResourceHandleHelper::FromJsonHelper(J, ResourceHandle);
}

