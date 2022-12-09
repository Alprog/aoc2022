#include "puzzle_handlers.h"

#include <fstream>
#include <format>

constexpr bool run_only_last_puzzle = false;

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

void run_puzzle(int day, int part, std::string& filename, std::string& fallback_filename, std::optional<std::string> answer)
{
	char part_letter = 'A' + part - 1;

	bool is_test = answer.has_value();
	std::cout << (is_test ? "Test" : "Puzzle") << part_letter << ": ";

	input input;
	input.current_part = part;
	read_all_lines(filename, input.lines);
	if (input.lines.empty())
	{
		read_all_lines(fallback_filename, input.lines);
	}
	if (input.lines.empty())
	{
		std::cout << "No Input\n";
		return;
	}
	
	output output;
	try_run_puzzle_handler(day, part, input, output);
	try_run_puzzle_handler(day, universal_part, input, output);
	if (!output.result.has_value())
	{
		std::cout << "No Handler\n";
		return;
	}

	const auto& result = output.result.value();
	std::cout << result;

	if (answer.has_value())
	{
		if (answer == "")
		{
			std::cout << " (answer unknown)";
		}
		else if (result == answer)
		{
			std::cout << " (passed)";
		}
		else
		{
			std::cout << " (failed!expected: " << answer.value() << ")";
		}
	}

	std::cout << std::endl;
}

void run_puzzle(int day)
{
	std::cout << "Day " << day << "\n";

	// Tests
	std::string answers_filename = std::format("input/{:02}_answers.txt", day);
	std::vector<std::string> answers;
	read_all_lines(answers_filename, answers);
	answers.resize(part_count);

	std::string test_fallback_filename = std::format("input/{:02}_test.txt", day);
	for (int part = 1; part <= part_count; part++)
	{
		std::string filename = std::format("input/{:02}_test_{:02}.txt", day, part);
		run_puzzle(day, part, filename, test_fallback_filename, answers[part - 1]);
	}

	// Puzzles
	std::string input_fallback_filename = std::format("input/{:02}_input.txt", day);
	for (int part = 1; part <= part_count; part++)
	{
		std::string filename = std::format("input/{:02}_input_{:02}.txt", day, part);
		run_puzzle(day, part, filename, input_fallback_filename, {});
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
			std::cout << "\n";
		}
	}	

	return 0;
}