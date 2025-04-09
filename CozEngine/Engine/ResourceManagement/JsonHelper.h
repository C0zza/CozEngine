#pragma once

#define NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_BASE(Type, Base, ...)  \
    friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { \
		to_json(nlohmann_json_j, static_cast<const Base&>(nlohmann_json_t));  \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) \
	} \
    friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { \
		from_json(nlohmann_json_j, static_cast<Base&>(nlohmann_json_t));  \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) \
	}
