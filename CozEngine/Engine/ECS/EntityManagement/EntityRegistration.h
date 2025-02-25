#pragma once

#include <string>
#include <memory>

#include "ECS/Entity.h"

#define REGISTER_ENTITY(TYPE) \
    static bool TYPE##_registered = []() { \
        LEntityRegister::RegisterFunc(#TYPE, []() { return std::make_unique<TYPE>(); }); \
        return true; \
    }();