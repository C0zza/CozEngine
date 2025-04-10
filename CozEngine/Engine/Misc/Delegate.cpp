#include "stdafx.h"
#include "Delegate.h"

LDelegateEventHandle::~LDelegateEventHandle()
{
	if (BoundDelegate)
	{
		BoundDelegate->RemoveEvent(this);
	}
}

void LDelegateEventHandle::Reset()
{
	if (BoundDelegate)
	{
		BoundDelegate->RemoveEvent(this);
	}
}
