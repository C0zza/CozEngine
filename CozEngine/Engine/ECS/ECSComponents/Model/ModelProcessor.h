#pragma once

#include "ECS/ECS2/EntityProcessor.h"

struct CModelComponent;
struct CTransformComponent;
class LShader;

class LDrawModeSubsystem;
class LSelectedEntitySubsystem;

REFL_CLASS()
class LModelProcessor : public LEntityProcessor
{
	REFL_GENERATED_BODY(LModelProcessor)
public:
	virtual void Initialize() override;
	virtual void ForEachEntityChunk(FEntityChunkHandle& EntityChunk) override;

private:
	void DrawModel(CModelComponent& ModelComponent, CTransformComponent& TransformComponent, const LShader* Shader);

#if defined(COZ_EDITOR)
	LDrawModeSubsystem* DrawModeSubsystem = nullptr;
	LSelectedEntitySubsystem* SelectedEntitySubsystem = nullptr;
#endif
};

