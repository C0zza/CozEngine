#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Input/InputManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

class Camera;
class Shader;
class Texture;
class Transform;
struct GLFWwindow;

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

	// Temp stuff while we don't have systems setup
	unsigned int VAOs[1]{ 0 };
	unsigned int VBOs[1]{ 0 };

	std::vector<glm::vec3> CubePositions;

	std::unique_ptr<Shader> DefaultShader;
	std::unique_ptr<Texture> SomeTexture;
	std::unique_ptr<Texture> SmileyTexture;

	std::shared_ptr<Camera> SomeCamera;

	glm::vec3 LightColor = glm::vec3(0);
};

