#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Input/InputManager.h"
#include "ECS/ECS.h"
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
	LInputManager InputManager;
	LECS ECS;

	std::vector<std::unique_ptr<CObject>> Objects;
	
	// TEMP
	LTransform* PointLightTransform;
};

