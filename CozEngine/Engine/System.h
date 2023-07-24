#pragma once

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

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
	void SetupGame();
	void Run();

private:
	GLFWwindow* Window;

	void ProcessInput(GLFWwindow* Window);

	// Temp stuff while we don't have systems setup
	unsigned int VAOs[1];
	unsigned int VBOs[1];

	std::vector<glm::vec3> CubePositions;

	std::unique_ptr<Shader> DefaultShader;
	std::unique_ptr<Texture> SomeTexture;
	std::unique_ptr<Texture> SmileyTexture;
	std::unique_ptr<Transform> SomeTransform;
	std::unique_ptr<Transform> CameraTransform;

	glm::vec3 CameraPosition;
	glm::vec3 CameraTarget;
	glm::vec3 CameraDirection;
	glm::vec3 Up;
	glm::vec3 CameraRight;
	glm::vec3 CameraUp;
};

