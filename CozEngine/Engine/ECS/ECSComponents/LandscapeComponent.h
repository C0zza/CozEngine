#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "ResourceManagement/ResourceHandle.h"

class LMesh;
class LShader;

struct CLandscapeComponent : public LECSComponent
{
	LResourceHandle<LTexture> HeightMap;
	LResourceHandle<LTexture> GroundTexture;
	LResourceHandle<LTexture> WallTexture;

	CLandscapeComponent() = default;
	CLandscapeComponent(const std::string& i_HeightMap, const std::string& i_GroundTexture, const std::string& i_WallTexture);
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

	// Plane mesh used for all landscape components.
	std::unique_ptr<LMesh> LandscapeMesh;

	int Width;
	int Height;
	int Length;
};

