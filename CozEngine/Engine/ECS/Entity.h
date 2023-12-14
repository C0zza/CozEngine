#pragma once

#include "ECS.h"
#include "Transform.h"

class LEntity
{
public:
	LEntity();
	~LEntity();

	LTransform& GetTransform() { return Transform; }
	LEntityID GetID() const { return ID; }

	template<typename TComponentType, typename... TInitArgs>
	TComponentType* AddComponent(TInitArgs... Args)
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
	// TODO: remove when everything is converted to ECS
	LTransform Transform;
	LEntityID ID;
};
