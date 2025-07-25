#pragma once

#include "ECS/ComponentSystem.h"
#include "ECS/ECSComponents/ECSComponent.h"
#include "Rendering/Materials/ModelMaterial.h"
#include "Rendering/Model.h"
#include "ResourceManagement/ResourceManager.h"

#if defined (COZ_EDITOR)
class LDrawModeSubsystem;
class LSelectedEntitySubsystem;
#endif

// TODO: rename to something decent once we can get rid of the existing CModelComponent
REFL_CLASS()
struct CModelComponent : public LECSComponent
{
	REFL_PROP(Visible)
	LResourceHandle<LModel> Model;
	REFL_PROP(Visible)
	LResourceHandle<LModelMaterial> ModelMaterial;

	CModelComponent() = default;
	CModelComponent(const std::string& i_Model, const std::string& i_ModelMaterial);

	REFL_GENERATED_BODY(CModelComponent)
};

class CModelComponentSystem : public LComponentSystem<CModelComponent>
{
public:
	virtual void RunComponent(CModelComponent& Component) override;

	virtual EComponentSystemType GetComponentSystemType() const { return EComponentSystemType::Renderer; }
	virtual const char* GetComponentName() const final { return "ModelComponentSystem"; }

protected:

	virtual void GetSerializedComponent(const CModelComponent& Component, nlohmann::json& J) const final;
	virtual void DeserializeComponent(CModelComponent& Component, const nlohmann::json& J) final;

#if defined(COZ_EDITOR)
private:
	// TODO: Possibly setup macros to define these much easer. Will also need to setup FOR_EACH_N macros
	virtual void DrawImGuiComponent(CModelComponent& Component) final;

	virtual void Init() final;

	LDrawModeSubsystem* DrawModeSubsystem = nullptr;
	LSelectedEntitySubsystem* SelectedEntitySubsystem = nullptr;
#endif
};

