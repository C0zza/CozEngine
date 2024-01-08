#pragma once

#include "ResourceManagement/Serializable.h"

class LSerializableTest : public LSavable
{
public:
	void Test();
	int SomeInt = 0;
};

