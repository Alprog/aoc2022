#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>

std::vector<int> get_signal_values(input& input)
{
	int value = 1;
	std::vector<int> values = { value };

	for (auto& line : input.lines)
	{
		if (line[0] == 'n')
		{
			values.push_back(value);
		}
		else
		{
			values.push_back(value);
			values.push_back(value);
			value += std::atoi(line.c_str() + 5);
		}
	}
	return values;
}

puzzle<10, 1> X = [](input& input) -> output
{
	std::vector<int> values = get_signal_values(input);

	int total = 0;
	std::vector<int> indices = { 20, 60, 100, 140, 180, 220 };
	for (auto index : indices)
	{
		total += index * values[index];
	}

	return total;
};

puzzle<10, 2> X = [](input& input) -> output
{
	std::vector<int> values = get_signal_values(input);

	int index = 0;
	std::cout << std::endl;
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 40; col++)
		{
			auto sprite_offset = values[++index];
			auto enabled = std::abs(sprite_offset - col) <= 1;
			std::cout << (enabled ? '#' : '.');
		}
		std::cout << std::endl;
	}

	return 0;
};