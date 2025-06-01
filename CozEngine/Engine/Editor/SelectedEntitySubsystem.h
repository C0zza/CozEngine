#pragma once

#include "Misc/Delegate.h"
#include "Subsystem.h"

using FEntitySelectedDelegate = LDelegate<const LEntityID>;

class LSelectedEntitySubsystem : public LSubsystem
{
public:
	LEntityID GetSelectedEntityID() const { return SelectedEntityID; }
	void SetSelectedEntityID(const LEntityID iSelectedEntityID);

	bool IsEntitySelected() const { return SelectedEntityID != 0; }

	FEntitySelectedDelegate& GetOnEntitySelectedDelegate() { return OnEntitySelectedDelegate; }

private:
	LEntityID SelectedEntityID = 0;

	FEntitySelectedDelegate OnEntitySelectedDelegate;
};

