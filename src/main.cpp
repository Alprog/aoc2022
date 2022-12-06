#include "puzzle_handlers.h"

#include <fstream>
#include <format>

constexpr bool run_only_last_puzzle = true;

constexpr int part_count = 2;

void run_puzzle(int day)
{
	input input;

	std::string input_file_name = std::format("input/{:02}.txt", day);

	std::ifstream file(input_file_name);
	std::string line;
	while (std::getline(file, line))
	{
		input.lines.push_back(line);
	}

	for (int part = 1; part <= part_count; part++)
	{
		input.current_part = part;
		try_run_puzzle_handler(day, universal_part, input);
		try_run_puzzle_handler(day, part, input);
	}
}

int main()
{
	if (run_only_last_puzzle)
	{
		run_puzzle(get_max_puzzle_day());
	}
	else
	{
		for (int i = 1; i <= get_max_puzzle_day(); i++)
		{
			run_puzzle(i);
		}
	}	

	return 0;
}