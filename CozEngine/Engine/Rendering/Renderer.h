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
	void GetShouldWindowClose(bool& bClose);

	void SetProjectionMatrix(const float Width, const float Height);
	const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; }

	void BindFrameBuffer(const LFrameBuffer* Buffer);
	void BindDefaultFrameBuffer();
	void ClearFrameBuffer(const float& R, const float& G, const float& B, const float& A, GLbitfield ClearFlags);

	void RegisterMatricesUBOToShader(const unsigned int ShaderID);
	void ReigsterLightingUBOToShader(const unsigned int ShaderID);

	template<typename T>
	void UpdateLightingUBOData(GLintptr Offset, GLsizeiptr Size, T& Value)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, LightingUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, Offset, Size, &Value);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

private:
	LWindow* m_Window;
	glm::mat4 ProjectionMatrix = glm::mat4(0);

	LFrameBuffer* CustomFrameBuffer = nullptr;

	LECS* ECS = nullptr;

	unsigned int MatricesUBO = 0;
	unsigned int LightingUBO = 0;
};



