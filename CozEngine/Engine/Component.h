#pragma once

#include <memory>

class CObject;

class CComponent
{
public:
	CComponent() {};
	virtual ~CComponent() {};

	void SetParent(CObject* Object) { Parent = Object; }

protected:
	CObject* Parent = nullptr;
};

