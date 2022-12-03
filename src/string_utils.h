#pragma once

#include <vector>
#include <string>

namespace str_utils
{
    template<typename T>
    bool contains(std::string_view line, T pattern, size_t offset = 0)
    {
        auto index = line.find(pattern, offset);
        return index != std::string::npos;
    }

    std::vector<std::string> split(std::string& line, std::string_view delimeter);
    std::string join(std::vector<std::string>& vector, std::string_view delimeter);
    void replace_all(std::string& string, std::string_view old_value, std::string_view new_value);
}