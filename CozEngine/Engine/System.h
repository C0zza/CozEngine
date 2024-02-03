#pragma once

#include "Input/InputManager.h"
#include "ECS/ECS.h"
#include "Rendering/Renderer.h"

class System
{
public:
	void Shutdown();
	void SetupGame();
	void Run();

private:
	Renderer m_Renderer;
	LInputManager InputManager;
	LECS ECS;
};

