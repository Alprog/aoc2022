#include "puzzles.h"

#include "string_utils.h"

void puzzle<4, 1>::run(std::vector<std::string>& lines)
{
	auto total = 0;

	const int arg_count = 4;

	int arg[arg_count];
	for (auto& line : lines)
	{
		str_utils::replace_all(line, ",", "-");
		auto arr = str_utils::split(line, "-");
		for (int i = 0; i < arg_count; i++)
		{
			arg[i] = std::atoi(arr[i].c_str());
		}

		auto a = arg[0] >= arg[2] && arg[1] <= arg[3];
		auto b = arg[0] <= arg[2] && arg[1] >= arg[3];
		if (a || b)
		{
			total++;
		}
	}

	std::cout << total << "\n";
}

void puzzle<4, 2>::run(std::vector<std::string>& lines)
{
	auto total = 0;

	const int arg_count = 4;

	int arg[arg_count];
	for (auto& line : lines)
	{
		str_utils::replace_all(line, ",", "-");
		auto arr = str_utils::split(line, "-");
		for (int i = 0; i < arg_count; i++)
		{
			arg[i] = std::atoi(arr[i].c_str());
		}

		auto a = arg[0] >= arg[3] && arg[1] <= arg[2];
		auto b = arg[0] <= arg[3] && arg[1] >= arg[2];
		if (a || b)
		{
			total++;
		}
	}

	std::cout << total << "\n";
}