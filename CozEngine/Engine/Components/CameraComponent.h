#pragma once

#include <glm/mat4x4.hpp>

#include "Component.h"

class CCameraComponent : public CComponent
{
public:
	void Init();

	const glm::mat4& GetViewMatrix();

	void ActivateCamera() { ActiveCamera = this; }

	virtual void Tick() override {}

	static CCameraComponent* GetActiveCamera() { return ActiveCamera; }

private:
	glm::mat4 CachedViewMatrix;

	static CCameraComponent* ActiveCamera;
};

