#include "PointLightComponent.h"

#include <sstream>

#include "ECS/ECS.h"
#include "Globes.h"
#include "Misc/GlmSerialization.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

#if defined(COZ_EDITOR)
#include "Editor/ImGuiPropertyDrawHelpers.h"
#endif

namespace CE::CPointLightComponent
{
	struct CPointLightComponentShaderStruct
	{
		void Update(const glm::vec3& Pos, const glm::vec3& Amb, const glm::vec3& Dif,
			const glm::vec3& Spec, const float& Const, const float& Lin, const float& Quad)
		{
			Position = Pos;
			Ambient = Amb;
			Diffuse = Dif;
			Specular = Spec;
			Constant = Const;
			Linear = Lin;
			Quadratic = Quad;
		}

		glm::vec3 Position;		// 0 - 12
		float padding1 = 0.f;
		glm::vec3 Ambient;		// 16 - 28
		float padding2 = 0.f;
		glm::vec3 Diffuse;		// 32 - 44
		float padding3 = 0.f;
		glm::vec3 Specular;		// 48 - 60
		float padding4 = 0.f;

		float Constant;		// 64 - 68
		float Linear;		// 68 - 72
		float Quadratic;	// 72 - 76
	};

	CPointLightComponentShaderStruct ShaderDatas[MAX_NUM_POINT_LIGHT];

	LRenderer* Renderer = nullptr;

	const int PointLightArrayOffset = 80;
	const int PointLightStructSize = 80;
}

CPointLightComponent::CPointLightComponent()
	: Constant{ -1.f }, Linear{ -1.f }, Quadratic{ -1.f }
{
	glm::vec3 ZeroVector = glm::vec3(0.f, 0.f, 0.f);
	Position = ZeroVector;
	Ambient = ZeroVector;
	Diffuse = ZeroVector;
	Specular = ZeroVector;
}

void CPointLightComponentSystem::UpdatePointLights()
{
	if (IsCountDirty)
	{
		using namespace CE::CPointLightComponent;
		if (!Renderer)
		{
			Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
		}

		Renderer->UpdateLightingUBOData(
			848,
			sizeof(int),
			PointLightCount);

		IsCountDirty = false;
	}

	assert(ECS);
	assert(PointLightCount <= MAX_NUM_POINT_LIGHT);

	for (int i = 0; i < PointLightCount; ++i)
	{
		if (CPointLightComponent* PointLightComp = ECS->GetComponent<CPointLightComponent>(PointLights[i]))
		{
			UpdatePointLight(PointLightComp, i);
		}
	}
}

#if defined(COZ_EDITOR)
void CPointLightComponentSystem::DrawImGuiComponent(CPointLightComponent& Component)
{
	LImGuiPropertyDrawHelpers::DrawProperty("Position", Component.Position);
	LImGuiPropertyDrawHelpers::DrawProperty("Ambient", Component.Ambient);
	LImGuiPropertyDrawHelpers::DrawProperty("Diffuse", Component.Diffuse);
	LImGuiPropertyDrawHelpers::DrawProperty("Specular", Component.Specular);

	LImGuiPropertyDrawHelpers::DrawProperty("Constant", Component.Constant);
	LImGuiPropertyDrawHelpers::DrawProperty("Linear", Component.Linear);
	LImGuiPropertyDrawHelpers::DrawProperty("Quadratic", Component.Quadratic);
}
#endif

void CPointLightComponentSystem::OnComponentAdded(CPointLightComponent& Component)
{
	if (PointLightCount < MAX_NUM_POINT_LIGHT)
	{
		++PointLightCount;
		IsCountDirty = true;
	}

	PointLights.push_back(Component.EntityID);
}

void CPointLightComponentSystem::OnComponentRemoved(CPointLightComponent& Component)
{
	std::vector<LEntityID>::iterator it = std::find(PointLights.begin(), PointLights.end(), Component.EntityID);
	
	int Index = it - PointLights.begin();

	if (Index >= MAX_NUM_POINT_LIGHT)
	{
		PointLights.erase(PointLights.begin() + Index);
	}
	else
	{
		if (Index == PointLights.size() - 1)
		{
			PointLights.erase(PointLights.begin() + Index);
			--PointLightCount;
			IsCountDirty = true;
		}
		else
		{
			PointLights[Index] = PointLights[PointLights.size() - 1];

			assert(ECS);
			CPointLightComponent* PointLightComp = ECS->GetComponent<CPointLightComponent>(PointLights[Index]);
			assert(PointLightComp);
			
			if (PointLights.size() <= MAX_NUM_POINT_LIGHT)
			{
				--PointLightCount;
				IsCountDirty = true;
			}

			PointLights.pop_back();
		}
	}
}

void CPointLightComponentSystem::GetSerializedComponent(const CPointLightComponent& Component, nlohmann::json& J) const
{
	J["Ambient"] = Component.Ambient;
	J["Constant"] = Component.Constant;
	J["Diffuse"] = Component.Diffuse;
	J["Linear"] = Component.Linear;
	J["Position"] = Component.Position;
	J["Quadratic"] = Component.Quadratic;
	J["Specular"] = Component.Specular;
}

void CPointLightComponentSystem::DeserializeComponent(CPointLightComponent& Component, const nlohmann::json& J)
{
	Component.Ambient = J["Ambient"];
	Component.Constant = J["Constant"];
	Component.Diffuse = J["Diffuse"];
	Component.Linear = J["Linear"];
	Component.Position = J["Position"];
	Component.Quadratic = J["Quadratic"];
	Component.Specular = J["Specular"];
}

void CPointLightComponentSystem::UpdatePointLight(CPointLightComponent* Component, int Index)
{
	assert(Component);
	assert(Index >= 0 && Index <= PointLightCount);
	
	CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(Component->EntityID);

	using namespace CE::CPointLightComponent;

	CPointLightComponentShaderStruct& ShaderData = ShaderDatas[Index];
	ShaderData.Update(
		TransformComp->GetPosition(),
		Component->Ambient,
		Component->Diffuse,
		Component->Specular,
		Component->Constant,
		Component->Linear,
		Component->Quadratic
	);

	Renderer->UpdateLightingUBOData(
		PointLightArrayOffset + PointLightStructSize * Index,
		sizeof(CPointLightComponentShaderStruct),
		ShaderData);
}
