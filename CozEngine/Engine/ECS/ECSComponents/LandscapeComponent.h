#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Rendering/Shader.h"
#include "Rendering/Materials/LandscapeMaterial.h"
#include "ResourceManagement/ResourceHandle.h"

class LMesh;

struct CLandscapeComponent : public LECSComponent
{
	LResourceHandle<LLandscapeMaterial> LandscapeMaterial;

	CLandscapeComponent() = default;
	CLandscapeComponent(const std::string& i_LandscapeMaterial);
};

class CLandscapeComponentSystem : public LComponentSystem<CLandscapeComponent>
{
public:
	CLandscapeComponentSystem();

	virtual void PreRun() override;
	virtual void RunComponent(CLandscapeComponent& Component) override;

	virtual EComponentSystemType GetComponentSystemType() const { return EComponentSystemType::Renderer; }
	virtual const char* GetComponentName() const final { return "LandscapeComponentSystem"; }

protected:

	virtual void GetSerializedComponent(const CLandscapeComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CLandscapeComponent& Component, const nlohmann::json& J) final;

private:
	void GenerateMesh();

	LResourceHandle<LShader> LandscapeShader;
#if defined(COZ_EDITOR)
	LResourceHandle<LShader> LandscapeEntityBufferShader;
#endif

	// Plane mesh used for all landscape components.
	std::unique_ptr<LMesh> LandscapeMesh;

	int Width;
	int Height;
	int Length;
};

