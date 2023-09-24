#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Input/InputManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

class CObject;
class Camera;
struct GLFWwindow;

// TEMP
class LTransform;

class System
{
public:
	System();
	~System();

	void Init();
	void Shutdown();
	void SetupGame();
	void Run();

private:
	Renderer m_Renderer;
	InputManager m_InputManager;

	std::vector<std::unique_ptr<CObject>> Objects;

	std::shared_ptr<Camera> SomeCamera;
	
	// TEMP
	LTransform* PointLightTransform;
};

