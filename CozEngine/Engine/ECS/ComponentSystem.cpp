#include "ComponentSystem.h"

#include "ECS.h"

LECS* LComponentSystemBase::ECS = nullptr;

void LComponentSystemBase::SetECS(LECS* i_ECS)
{
	ECS = i_ECS;
}