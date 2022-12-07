#include "puzzle_handlers.h"

#include <fstream>
#include <format>

constexpr bool run_only_last_puzzle = true;

constexpr int part_count = 2;

void read_all_lines(std::string& filename, std::vector<std::string>& output)
{
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line))
	{
		output.push_back(line);
	}
}

void run_puzzle(int day, std::string& input_filename, std::string answers_filename)
{
	input input;
	read_all_lines(input_filename, input.lines);

	bool is_test = answers_filename != "";

	if (input.lines.empty())
	{
		std::cout << (is_test ? "Test: No Input\n" : "Puzzle: No Input\n");
		return;
	}

	std::vector<std::string> answers;
	read_all_lines(answers_filename, answers);

	for (int part = 1; part <= part_count; part++)
	{
		input.current_part = part;
		
		output output;
		try_run_puzzle_handler(day, part, input, output);
		try_run_puzzle_handler(day, universal_part, input, output);
		
		char part_letter = 'A' + part - 1;

		if (output.result.has_value())
		{
			const auto& value = output.result.value();
			if (is_test)
			{
				if (answers.size() < part)
				{
					std::cout << std::format("Test{}: {} (answer unknown)\n", part_letter, value);
					continue;
				}

				auto answer = answers[part - 1];
				if (output.result == answer)
				{
					std::cout << std::format("Test{}: {} (passed)\n", part_letter, value);
				}
				else
				{
					std::cout << std::format("Test{}: {} (failed! expected: {})\n", part_letter, value, answer);
				}
			}
			else
			{
				std::cout << std::format("Part{}: {}\n", part_letter, value);
			}
		}
	}
}

void run_puzzle(int day)
{
	std::cout << "Day " << day << "\n";

	std::string test_filename = std::format("input/{:02}_test.txt", day);
	std::string answers_filename = std::format("input/{:02}_answers.txt", day);
	run_puzzle(day, test_filename, answers_filename);

	std::string input_filename = std::format("input/{:02}_input.txt", day);
	run_puzzle(day, input_filename, "");
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
			std::cout << "\n";
		}
	}	

	return 0;
}