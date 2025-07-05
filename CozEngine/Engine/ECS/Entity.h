#pragma once

#include <memory>

#include "EntityManagement/EntityRegistration.h"
#include "ECS.h"
#include "Globes.h"
#include "Reflection/Reflection.h"

REFL_CLASS(Abstract)
class LEntity
{
	REFL_GENERATED_BODY(LEntity)
public:
	LEntity();
	virtual ~LEntity();

	LEntityID GetID() const { return ID; }

	template<typename TComponentType, typename... TInitArgs>
	TComponentType* AddComponent(TInitArgs&... Args)
	{
		return GetECS()->AddComponent<TComponentType>(ID, Args...);
	}

	template<typename TComponentType>
	void RemoveComponent()
	{
		GetECS()->RemoveComponent<TComponentType>(ID);
	}

	template<typename TComponentType>
	TComponentType* GetComponent()
	{
		return GetECS()->GetComponent<TComponentType>(ID);
	}

#if defined(COZ_EDITOR)
	virtual const char* GetTypeName() const { return "Entity"; }
#endif

protected:
	LEntityID ID;

	LECS* GetECS() const;
};

using LEntityRegister = LFuncRegister<std::unique_ptr<LEntity>>;