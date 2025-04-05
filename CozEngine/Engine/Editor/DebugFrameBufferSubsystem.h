#pragma once

#include <string>

#include "Subsystem.h"

class LDebugFrameBufferSubsystem : public LSubsystem
{
public:
	void SetActiveDebugFrameBuffer(const std::string& iActiveDebugFrameBuffer) { ActiveDebugFrameBuffer = iActiveDebugFrameBuffer; }
	const std::string& GetActiveDebugFrameBuffer() const { return ActiveDebugFrameBuffer; }

private:
	std::string ActiveDebugFrameBuffer = "";
};

