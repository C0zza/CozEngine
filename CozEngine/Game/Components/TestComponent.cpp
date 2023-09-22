#include "TestComponent.h"

#include "Object.h"
#include "Transform.h"

void CTestComponent::Tick()
{
	assert(Parent);
	Parent->Transform.Rotate(glm::vec3(.0f, .002f, .0f));
}
