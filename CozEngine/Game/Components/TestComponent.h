#pragma once
#include "Engine/Components/Component.h"

class CTestComponent : public CComponent
{
public:
	CTestComponent() {}
	virtual ~CTestComponent() {};

	virtual void Tick() override;
};

