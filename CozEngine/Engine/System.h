#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Input/InputManager.h"
#include "ECS/ECS.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

class System
{
public:
	void Init();
	void Shutdown();
	void SetupGame();
	void Run();

private:
	Renderer m_Renderer;
	LInputManager InputManager;
	LECS ECS;
};

