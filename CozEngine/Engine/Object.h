#pragma once

#include <memory>
#include <vector>

#include "Components/ComponentList.h"
#include "Transform.h"

class CObject
{
public:
	virtual void TickComponents();

	// Components to manage the behaviour of different CObjects.
	CComponentList Components;

	LTransform Transform;
};
