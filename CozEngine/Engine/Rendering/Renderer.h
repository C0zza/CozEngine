#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "Subsystem.h"

class LECS;
class LFrameBuffer;
class LRendererInfo;
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

	void BindFrameBuffer(const LFrameBuffer* Buffer);
	void BindDefaultFrameBuffer();
	void ClearFrameBuffer(const float& R, const float& G, const float& B, const float& A, GLbitfield ClearFlags);

	void RegisterMatricesUBOToShader(const unsigned int ShaderID);
	void ReigsterLightingUBOToShader(const unsigned int ShaderID);

	template<typename T>
	void UpdateLightingUBOData(GLintptr Offset, GLsizeiptr Size, const T& Value)
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

	LFrameBuffer* CustomFrameBuffer = nullptr;

	LRendererInfo* RendererInfo = nullptr;

	LECS* ECS = nullptr;

	unsigned int MatricesUBO = 0;
	unsigned int LightingUBO = 0;
};



