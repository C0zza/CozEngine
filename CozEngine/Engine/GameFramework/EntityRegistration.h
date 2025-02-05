#pragma once

#include <string>
#include <memory>

#include "EntityFactory.h"

#define REGISTER_ENTITY(TYPE) \
    static bool TYPE##_registered = []() { \
        LEntityFactory::RegisterEntity(#TYPE, []() { return std::make_unique<TYPE>(); }); \
        return true; \
    }()
