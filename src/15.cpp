#include "puzzle_handlers.h"

#include "string_utils.h"
#include "vector2.h"
#include <functional>

puzzle<15, 1> X = [](input& input) -> output
{
	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, "=");
		auto sx = std::atoi(arr[1].c_str());
		auto sy = std::atoi(arr[2].c_str());
		auto bx = std::atoi(arr[3].c_str());
		auto by = std::atoi(arr[4].c_str());
	}

	return 0;
};