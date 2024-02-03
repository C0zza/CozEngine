#pragma once

#include "ECS.h"

class LEntity
{
public:
	LEntity();
	~LEntity();

	LEntityID GetID() const { return ID; }

	template<typename TComponentType, typename... TInitArgs>
	TComponentType* AddComponent(TInitArgs&... Args)
	{
		return LECS::Get()->AddComponent<TComponentType>(ID, Args...);
	}

	template<typename TComponentType>
	void RemoveComponent()
	{
		LECS::Get()->RemoveComponent<TComponentType>(ID);
	}

	template<typename TComponentType>
	TComponentType* GetComponent()
	{
		return LECS::Get()->GetComponent<TComponentType>(ID);
	}

protected:
	LEntityID ID;
};
