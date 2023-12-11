#pragma once

#include "ECS.h"
#include "Transform.h"

class LEntity
{
public:
	LEntity(LECS* i_ECS);
	~LEntity();

	LTransform& GetTransform() { return Transform; }
	LEntityID GetID() const { return ID; }

protected:
	template<typename TComponentType, typename... TInitArgs>
	void AddComponent(TInitArgs... Args)
	{
		ECS->AddComponent<TComponentType>(ID, Args...);
	}

	template<typename TComponentType>
	void RemoveComponent()
	{
		ECS->RemoveComponent<TComponentType>(ID);
	}

	template<typename TComponentType>
	bool GetComponent(TComponentType& Component)
	{
		return ECS->GetComponent<TComponentType>(ID, Component);
	}

	LTransform Transform;

private:
	LEntityID ID;
	LECS* ECS;
};
