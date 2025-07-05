#pragma once

#include <set>

class LClass;

struct FArchetypeSignature
{
public:
	std::string ToString() const;
	static FArchetypeSignature FromString(const std::string& Signature);

	bool operator==(const FArchetypeSignature& Other) const = default;

	struct FClassSetComparison
	{
		bool operator()(const LClass* A, const LClass* B) const;
	};

	std::set<LClass*, FClassSetComparison> Classes;
};

