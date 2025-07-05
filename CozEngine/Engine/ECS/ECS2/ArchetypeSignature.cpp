#include "stdafx.h"

#include "ArchetypeSignature.h"

#include "Reflection/Class.h"

bool FArchetypeSignature::FClassSetComparison::operator()(const LClass* A, const LClass* B) const
{
    assert(A && B);
    return A->GetByteSize() > B->GetByteSize();
}

std::string FArchetypeSignature::ToString() const
{
    return std::string();
}

FArchetypeSignature FArchetypeSignature::FromString(const std::string& Signature)
{
    return FArchetypeSignature();
}