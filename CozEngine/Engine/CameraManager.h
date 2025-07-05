#pragma once

#include "Subsystem.h"

class LArchetypeManager;

class LCameraManager : public LSubsystem
{
public:
	virtual void Initialize() override;

	const glm::mat4& GetViewMatrix();
	const glm::vec3& GetViewPos();

	void ActivateCamera(const LEntityID EntityID) { ActiveCameraEntityID = EntityID; }
	int GetActiveCameraEntityID() const { return ActiveCameraEntityID; }

private:
	LEntityID ActiveCameraEntityID = 0;

	LArchetypeManager* ArchetypeManager = nullptr;
};

