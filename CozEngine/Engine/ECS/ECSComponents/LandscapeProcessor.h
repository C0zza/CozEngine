#pragma once

#include "ECS/ECS2/EntityProcessor.h"
#include "ResourceManagement/ResourceHandle.h"

class LMesh;
class LShader;

REFL_CLASS()
class LLandscapeProcessor : public LEntityProcessor
{
	REFL_GENERATED_BODY(LLandscapeProcessor)
public:
	LLandscapeProcessor();
	~LLandscapeProcessor();

	virtual void Initialize() override;
	virtual void Execute(FEntityQueryResult& EntityQueryResult) override;

private:
	void GenerateMesh();

	LResourceHandle<LShader> LandscapeShader;
#if defined(COZ_EDITOR)
	LResourceHandle<LShader> LandscapeEntityBufferShader;
#endif

	// Plane mesh used for all landscape components.
	std::unique_ptr<LMesh> LandscapeMesh = nullptr;

	int Width = 512;
	int Height = 100;
	int Length = 512;
};

