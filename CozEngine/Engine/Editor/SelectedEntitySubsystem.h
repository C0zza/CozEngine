#pragma once

#include "Misc/Delegate.h"
#include "Subsystem.h"

class LSelectedEntitySubsystem : public LSubsystem
{
public:
	LEntityID GetSelectedEntityID() const { return SelectedEntityID; }
	void SetSelectedEntityID(const LEntityID iSelectedEntityID);

	bool IsEntitySelected() const { return SelectedEntityID != 0; }

	LDelegate<const LEntityID>& GetOnEntitySelectedDelegate() { return OnEntitySelectedDelegate; }

private:
	LEntityID SelectedEntityID = 0;

	LDelegate<const LEntityID> OnEntitySelectedDelegate;
};

