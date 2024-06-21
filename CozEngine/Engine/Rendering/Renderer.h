#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "Subsystem.h"

class LECS;
class LFrameBuffer;
class LWindow;

class LRenderer : public LSubsystem
{
public:
	~LRenderer();

	virtual bool HasRequiredSubsystems() const override;

	void Update();
	void Swap();

	LWindow* GetWindow() { return m_Window; }
	const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }

	void BindCustomFrameBuffer();
	void UnbindCustomFrameBuffer();

	void SetCustomFrameBuffer(LFrameBuffer* FrameBuffer) { CustomFrameBuffer = FrameBuffer; }

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

private:
	LWindow* m_Window;
	glm::mat4 ProjectionMatrix = glm::mat4(0);

	LFrameBuffer* CustomFrameBuffer = nullptr;

	LECS* ECS = nullptr;
};



