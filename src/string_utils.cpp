#include "string_utils.h"

#include <sstream>

std::vector<std::string> str_utils::split(std::string& line, std::string_view delimeter)
{
    std::vector<std::string> result;

    size_t start = 0;
    size_t index = line.find(delimeter);
    while (index != std::string::npos)
    {
        result.push_back(line.substr(start, index - start));
        start = index + delimeter.size();
        index = line.find(delimeter, start);
    }
    if (start <= line.size())
    {
        result.push_back(line.substr(start));
    }

    return result;
}

std::string str_utils::join(std::vector<std::string>& vector, std::string_view delimeter)
{
    std::stringstream stream;

    auto size = vector.size();
    if (size > 0)
    {
        stream << vector[0];
        for (auto i = 1; i < size; i++)
        {
            stream << delimeter << vector[i];
        }
    }

    return stream.str();
}

void str_utils::replace_all(std::string& string, std::string_view old_value, std::string_view new_value)
{
    while (true)
    {
        auto index = string.find(old_value);
        if (index == std::string::npos)
        {
            break;
        }
        string.replace(index, old_value.size(), new_value);
    }
}