#include "TestComponent.h"

#include "Object.h"
#include "Transform.h"

void CTestComponent::Tick()
{
	assert(Parent);
	Parent->Transform.Rotate(glm::vec3(0.f, .01f, 0.f));
}
