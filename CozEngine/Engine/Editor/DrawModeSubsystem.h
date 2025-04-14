#pragma once

#include "json.hpp"
#include "Subsystem.h"

enum class EDrawMode
{
	Default,
	EntityFrameBuffer,
	Outline,
	// TODO: Add assertions to setter making sure this is never used
	NumDrawModes
};

NLOHMANN_JSON_SERIALIZE_ENUM(EDrawMode, 
{
	{EDrawMode::Default, "Default"},
	{EDrawMode::EntityFrameBuffer, "EntityFrameBuffer"},
	{EDrawMode::Outline, "Outline"},
	{EDrawMode::NumDrawModes, "NumDrawModes"}
})

class LDrawModeSubsystem : public LSubsystem
{
public:
	void SetActiveDrawMode(const EDrawMode iActiveDrawMode) { ActiveDrawMode = iActiveDrawMode; }
	EDrawMode GetActiveDrawMode() const { return ActiveDrawMode; }

private:
	EDrawMode ActiveDrawMode = EDrawMode::Default;
};

