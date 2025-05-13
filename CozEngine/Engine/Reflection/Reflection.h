#pragma once

#define REFL_CLASS() 
#define REFL_PROP()

#define REFL_GENERATED_BODY()	\
public:									\
	static class LClass* StaticClass(); \
private:								\
	static LClass* Class;