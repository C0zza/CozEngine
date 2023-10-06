#pragma once

#include <bitset>
#include <string>

using LEntity = std::size_t;
const LEntity MAX_ENTITIES = 5000;

using LComponentType = std::size_t;
const LComponentType MAX_COMPONENTS = 32;

using LSignature = std::bitset<MAX_COMPONENTS>;