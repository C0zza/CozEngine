#pragma once

class LDirtyFlag
{
public:
	bool GetIsDirty() const { return IsDirty; }
	void ClearDirtyFlag() { IsDirty = false; }

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

