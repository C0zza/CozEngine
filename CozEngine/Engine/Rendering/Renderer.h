#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "Subsystem.h"

class LECS;
class LWindow;

class LRenderer : public LSubsystem<LRenderer>
{
public:
	virtual bool HasRequiredSubsystems() const override;

	void Update();
	void Swap();

	LWindow* GetWindow() { return m_Window.get(); }
	const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

private:
	std::unique_ptr<LWindow> m_Window;
	glm::mat4 ProjectionMatrix = glm::mat4(0);

	LECS* ECS = nullptr;
};



