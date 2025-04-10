#include "stdafx.h"
#include "SelectedEntitySubsystem.h"

void LSelectedEntitySubsystem::SetSelectedEntityID(const LEntityID iSelectedEntityID)
{
	if (iSelectedEntityID == SelectedEntityID)
	{
		return;
	}

	SelectedEntityID = iSelectedEntityID;

	OnEntitySelectedDelegate.Broadcast(SelectedEntityID);
}
