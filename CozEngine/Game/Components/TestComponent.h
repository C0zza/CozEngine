#pragma once
#include "Engine/Component.h"

class CTestComponent : public CComponent
{
public:
	CTestComponent(int i_x, int i_y) 
	{
		x = i_x; y = i_y;
	}

	virtual ~CTestComponent() {};

private:
	int x;
	int y;
};

