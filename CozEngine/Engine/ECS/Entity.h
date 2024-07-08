#pragma once

#include "ECS.h"
#include "Globes.h"

class LEntity
{
public:
	LEntity();
	~LEntity();

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

protected:
	LEntityID ID;

	LECS* GetECS() const;
};
