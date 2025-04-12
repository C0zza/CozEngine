#include "SpotLightComponent.h"

#include <sstream>

#include "ECS/ECS.h"
#include "Globes.h"
#include "Misc/GlmSerialization.h"
#include "TransformComponent.h"
#include "Rendering/Lighting/Lighting.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

namespace CE::CSpotLightComponent
{
	struct CSpotLightComponentShaderStruct
	{
		void Update(const glm::vec3& Pos, const glm::vec3& Amb, const glm::vec3& Dif, const glm::vec3& Spec,
			const glm::vec3& Dir, const float& COff, const float& OutCOff, const float& Const, const float& Lin,
			const float& Quad)
		{
			Position = Pos;
			Ambient = Amb;
			Diffuse = Dif;
			Specular = Spec;
			Direction = Dir;
			CutOff = COff;
			OuterCutOff = OutCOff;
			Constant = Const;
			Linear = Lin;
			Quadratic = Quad;
		}

		glm::vec3 Position;		// 0 - 12
		float Padding1;
		glm::vec3 Ambient;		// 16 - 28
		float Padding2;
		glm::vec3 Diffuse;		// 32 - 44
		float Padding3;
		glm::vec3 Specular;		// 48 - 60
		float Padding4;
		glm::vec3 Direction;	// 64 - 76
		float Padding5;

		float CutOff;		// 80 - 84
		float OuterCutOff;  // 84 - 88
		float Constant;		// 88 - 92
		float Linear;		// 92 - 96
		float Quadratic;    // 96 - 100
	};

	CSpotLightComponentShaderStruct ShaderDatas[MAX_NUM_SPOT_LIGHT];

	const int SpotLightArrayOffset = 400;
	const int SpotLightStructSize = 112;

	LRenderer* Renderer = nullptr;
}

CSpotLightComponent::CSpotLightComponent()
	: CutOff{ -1.f }, OuterCutOff{ -1.f }, Constant{ -1.f }, Linear{ -1.f }, Quadratic{ -1.f }
{
}

void CSpotLightComponentSystem::UpdateSpotLights()
{
	if (IsCountDirty)
	{
		using namespace CE::CSpotLightComponent;
		if (!Renderer)
		{
			Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
		}

		Renderer->UpdateLightingUBOData(
			SpotLightArrayOffset + SpotLightStructSize * MAX_NUM_SPOT_LIGHT + sizeof(int),
			sizeof(int),
			SpotLightCount);

		IsCountDirty = false;
	}

	assert(ECS);
	assert(SpotLightCount <= MAX_NUM_SPOT_LIGHT);

	for (int i = 0; i < SpotLightCount; ++i)
	{
		if (CSpotLightComponent* SpotLightComp = ECS->GetComponent<CSpotLightComponent>(SpotLights[i]))
		{
			UpdateSpotLight(SpotLightComp, i);
		}
	}
}

void CSpotLightComponentSystem::OnComponentAdded(CSpotLightComponent& Component)
{
	if (SpotLightCount < MAX_NUM_SPOT_LIGHT)
	{
		++SpotLightCount;
		IsCountDirty = true;
	}

	SpotLights.push_back(Component.EntityID);
}

void CSpotLightComponentSystem::OnComponentRemoved(CSpotLightComponent& Component)
{
	std::vector<LEntityID>::iterator it = std::find(SpotLights.begin(), SpotLights.end(), Component.EntityID);

	int Index = it - SpotLights.begin();

	if (Index >= MAX_NUM_SPOT_LIGHT)
	{
		SpotLights.erase(SpotLights.begin() + Index);
	}
	else
	{
		if (Index == SpotLights.size() - 1)
		{
			SpotLights.erase(SpotLights.begin() + Index);
			--SpotLightCount;
			IsCountDirty = true;
		}
		else
		{
			SpotLights[Index] = SpotLights[SpotLights.size() - 1];

			assert(ECS);
			CSpotLightComponent* SpotLightComp = ECS->GetComponent<CSpotLightComponent>(SpotLights[Index]);
			assert(SpotLightComp);

			if (SpotLights.size() <= MAX_NUM_SPOT_LIGHT)
			{
				--SpotLightCount;
				IsCountDirty = true;
			}

			SpotLights.pop_back();
		}
	}
}

void CSpotLightComponentSystem::GetSerializedComponent(const CSpotLightComponent& Component, nlohmann::json& J) const
{
	J["Direction"] = Component.Direction;
	J["Ambient"] = Component.Ambient;
	J["Constant"] = Component.Constant;
	J["Diffuse"] = Component.Diffuse;
	J["Linear"] = Component.Linear;
	J["Position"] = Component.Position;
	J["Quadratic"] = Component.Quadratic;
	J["Specular"] = Component.Specular;
	J["CutOff"] = Component.CutOff;
	J["OuterCutOff"] = Component.OuterCutOff;
}

void CSpotLightComponentSystem::DeserializeComponent(CSpotLightComponent& Component, const nlohmann::json& J)
{
	Component.Direction = J["Direction"];
	Component.Ambient = J["Ambient"];
	Component.Constant = J["Constant"];
	Component.Diffuse = J["Diffuse"];
	Component.Linear = J["Linear"];
	Component.Position = J["Position"];
	Component.Quadratic = J["Quadratic"];
	Component.Specular = J["Specular"];
	Component.CutOff = J["CutOff"];
	Component.OuterCutOff = J["OuterCutOff"];
}

void CSpotLightComponentSystem::UpdateSpotLight(CSpotLightComponent* Component, int Index)
{
	assert(Component);
	assert(Index >= 0 && Index <= SpotLightCount);

	CTransformComponent* TransformComp = ECS->GetComponent<CTransformComponent>(Component->EntityID);

	using namespace CE::CSpotLightComponent;

	CSpotLightComponentShaderStruct& ShaderData = ShaderDatas[Index];
	ShaderData.Update(
		TransformComp->GetPosition(),
		Component->Ambient,
		Component->Diffuse,
		Component->Specular,
		TransformComp->GetForward(),
		Component->CutOff,
		Component->OuterCutOff,
		Component->Constant,
		Component->Linear,
		Component->Quadratic
	);

	Renderer->UpdateLightingUBOData(
		SpotLightArrayOffset + SpotLightStructSize * Index,
		sizeof(CSpotLightComponentShaderStruct),
		ShaderData);
}