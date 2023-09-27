#pragma once

#include <memory>

class CObject;

class CComponent
{
public:
	CComponent() {};
	virtual ~CComponent() {};

	virtual void Tick() = 0;

	CObject* Parent = nullptr;
};

