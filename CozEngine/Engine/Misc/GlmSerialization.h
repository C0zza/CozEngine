#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "json.hpp"

namespace glm
{
    // inline required to prevent multiple definition LNK errors
	inline void to_json(nlohmann::json& j, const glm::vec3& v) {
		j = nlohmann::json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
	}

	// Convert JSON to glm::vec3
	inline void from_json(const nlohmann::json& j, glm::vec3& v) {
		v.x = j.at("x").get<float>();
		v.y = j.at("y").get<float>();
		v.z = j.at("z").get<float>();
	}

    inline void to_json(nlohmann::json& j, const glm::mat4& mat) 
    {
        j = nlohmann::json::array();
        const float* p = glm::value_ptr(mat);
        for (int i = 0; i < 16; ++i) 
        {
            j.push_back(p[i]);
        }
    }

    inline void from_json(const nlohmann::json& j, glm::mat4& mat) 
    {
        if (!j.is_array() || j.size() != 16) 
        {
            throw std::runtime_error("Invalid JSON format for glm::mat4");
        }
        float values[16];
        for (int i = 0; i < 16; ++i) 
        {
            values[i] = j[i].get<float>();
        }
        mat = glm::make_mat4(values);
    }
}
