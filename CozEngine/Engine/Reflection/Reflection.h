#pragma once

#include "json.hpp"

#define REFL_CLASS(...) 
#define REFL_PROP(...)

#define REFL_GENERATED_BODY(Type)										\
public:																	\
	static class LClass* StaticClass();									\
private:																\
	static LClass* Class;												\
																		\
	friend void from_json(const nlohmann::json& Json, Type& Object);	\
	friend void to_json(nlohmann::json& Json, const Type& Object);