#include "Engine/System.h"

int main()
{
	std::unique_ptr<System> CozEngineSystem = std::make_unique<System>();

	CozEngineSystem->SetupGame();
	CozEngineSystem->Run();

	return 0;
}