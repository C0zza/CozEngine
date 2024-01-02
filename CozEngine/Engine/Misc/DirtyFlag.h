#pragma once

class LDirtyFlag
{
protected:
	template<typename T>
	void SetDirtyMember(T& Member, const T Update)
	{
		if (Member != Update)
		{
			Member = Update;
			IsDirty = true;
		}
	}

	bool IsDirty = true;
};

