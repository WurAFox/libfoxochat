#pragma once
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

namespace foxogram {

    class Utils {
    public:
        template <typename T>
        static T value(const nlohmann::json& j, const std::string& at, T default_value) {
            return (j.at(at).is_null()) ? default_value : j.at(at).get<T>();
        }
    };

}
