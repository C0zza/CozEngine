#include "stdafx.h"

#include "DirectionalLightSubsystem.h"

#include "ECS/ECS2/ArchetypeManager.h"
#include "ECS/ECSComponents/DirectionalLightComponent.h"
#include "ECS/ECSComponents/PointLightComponent.h"
#include "ECS/ECSComponents/SpotLightComponent.h"
#include "ECS/ECSComponents/TransformComponent.h"
#include "Globes.h"
#include "Rendering/Renderer.h"

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

	CSpotLightComponentShaderStruct ShaderDatas[MAX_NUM_SPOT_LIGHTS];

	const int SpotLightArrayOffset = 400;
	const int SpotLightStructSize = 112;
}

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

	CPointLightComponentShaderStruct ShaderDatas[MAX_NUM_POINT_LIGHTS];

	const int PointLightArrayOffset = 80;
	const int PointLightStructSize = 80;
}

void LDirectionalLightSubsystem::Initialize()
{
	Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	assert(Renderer);

	ArchetypeManager = CSystem.GetSubsystems().GetSubsystem<LArchetypeManager>();
	assert(ArchetypeManager);
}

void LDirectionalLightSubsystem::SetSpotLightEntityIDs(std::vector<LEntityID> EntityIDs)
{
	assert(EntityIDs.size() <= MAX_NUM_POINT_LIGHTS);

	SpotLightCount = EntityIDs.size();
	for (int i = 0; i < SpotLightCount; ++i)
	{
		SpotLightIDs[i] = EntityIDs[i];
	}
}

void LDirectionalLightSubsystem::SetPointLightEntityIDs(std::vector<LEntityID> EntityIDs)
{
	assert(EntityIDs.size() <= MAX_NUM_POINT_LIGHTS);

	PointLightCount = EntityIDs.size();
	for (int i = 0; i < PointLightCount; ++i)
	{
		PointLightIDs[i] = EntityIDs[i];
	}
}

void LDirectionalLightSubsystem::UpdateDirectionalLightData()
{
	if (!ActiveDirectionalLight)
	{
		return;
	}

	CDirectionalLightComponent* DirectionalLight = ArchetypeManager->GetEntityComponent<CDirectionalLightComponent>(ActiveDirectionalLight);
	if (!DirectionalLight)
	{
		ActiveDirectionalLight = 0;
		SetInvalidUBOData();
		return;
	}

	CTransformComponent* Transform = ArchetypeManager->GetEntityComponent<CTransformComponent>(ActiveDirectionalLight);
	if (!Transform)
	{
		SetInvalidUBOData();
		return;
	}

	std::stringstream DirectionalLightVar;
	DirectionalLightVar << "DirectionalLight.";

	SetUBOData(Transform->GetForward(), DirectionalLight->Ambient, DirectionalLight->Diffuse, DirectionalLight->Specular);
	// IsActive
	Renderer->UpdateLightingUBOData(64, sizeof(bool), true);
}

void LDirectionalLightSubsystem::UpdateSpotLightData()
{
	using namespace CE::CSpotLightComponent;

	Renderer->UpdateLightingUBOData(
		SpotLightArrayOffset + SpotLightStructSize * MAX_NUM_SPOT_LIGHTS + sizeof(int),
		sizeof(int),
		SpotLightCount);

	for (int i = 0; i < SpotLightCount; ++i)
	{
		CSpotLightComponent* SpotLightComponent = ArchetypeManager->GetEntityComponent<CSpotLightComponent>(SpotLightIDs[i]);
		CTransformComponent* TransformComponent = ArchetypeManager->GetEntityComponent<CTransformComponent>(SpotLightIDs[i]);
		if (SpotLightComponent)
		{
			UpdateSpotLight(SpotLightComponent, TransformComponent, i);
		}
	}
}

void LDirectionalLightSubsystem::UpdatePointLightData()
{
	using namespace CE::CPointLightComponent;

	Renderer->UpdateLightingUBOData(
		PointLightArrayOffset + PointLightStructSize * MAX_NUM_POINT_LIGHTS + sizeof(int),
		sizeof(int),
		PointLightCount);

	for (int i = 0; i < PointLightCount; ++i)
	{
		CPointLightComponent* PointLightComponent = ArchetypeManager->GetEntityComponent<CPointLightComponent>(PointLightIDs[i]);
		CTransformComponent* TransformComponent = ArchetypeManager->GetEntityComponent<CTransformComponent>(PointLightIDs[i]);
		if (PointLightComponent)
		{
			UpdatePointLight(PointLightComponent, TransformComponent, i);
		}
	}
}

void LDirectionalLightSubsystem::UpdateSpotLight(CSpotLightComponent* Component, CTransformComponent* Transform, int Index)
{
	using namespace CE::CSpotLightComponent;

	CSpotLightComponentShaderStruct& ShaderData = ShaderDatas[Index];
	ShaderData.Update(
		Transform->GetPosition(),
		Component->Ambient,
		Component->Diffuse,
		Component->Specular,
		Transform->GetForward(),
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

void LDirectionalLightSubsystem::UpdatePointLight(CPointLightComponent* PointLight, CTransformComponent* Transform, int Index)
{
	using namespace CE::CPointLightComponent;

	CPointLightComponentShaderStruct& ShaderData = ShaderDatas[Index];
	ShaderData.Update(
		Transform->GetPosition(),
		PointLight->Ambient,
		PointLight->Diffuse,
		PointLight->Specular,
		PointLight->Constant,
		PointLight->Linear,
		PointLight->Quadratic
	);

	Renderer->UpdateLightingUBOData(
		PointLightArrayOffset + PointLightStructSize * Index,
		sizeof(CPointLightComponentShaderStruct),
		ShaderData);
}

void LDirectionalLightSubsystem::SetUBOData(const glm::vec3& Direction, const glm::vec3& Ambient, const glm::vec3& Diffuse, const glm::vec3& Specular)
{
	// TODO: These offsets should be constants somewhere
	Renderer->UpdateLightingUBOData(0, sizeof(glm::vec3), Direction);
	Renderer->UpdateLightingUBOData(16, sizeof(glm::vec3), Ambient);
	Renderer->UpdateLightingUBOData(32, sizeof(glm::vec3), Diffuse);
	Renderer->UpdateLightingUBOData(48, sizeof(glm::vec3), Specular);
}

void LDirectionalLightSubsystem::SetInvalidUBOData()
{
	glm::vec3 ZeroVec = glm::vec3(0.f);
	SetUBOData(ZeroVec, ZeroVec, ZeroVec, ZeroVec);
	Renderer->UpdateLightingUBOData(64, sizeof(bool), false);
}
