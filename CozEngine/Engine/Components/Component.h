#pragma once

#include <memory>

class CObject;

class CComponent
{
public:
	CComponent() {};
	virtual ~CComponent() {};

	virtual void Tick() = 0;

	void SetParent(CObject* Object) { Parent = Object; }

protected:
	CObject* Parent = nullptr;
};

