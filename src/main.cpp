
#include "puzzles.h"

#include <fstream>
#include <format>
#include <type_traits>

constexpr int puzzle_count = 1;
constexpr int active_puzzle_index = 1;

template<int... indicies>
auto& get_puzzle_entry_points(std::integer_sequence<int, indicies...> sequence)
{
	static std::vector<puzzle_entry_point> entry_points = { puzzle<indicies + 1>::run... };
	return entry_points;
}

puzzle_entry_point get_puzzle_entry_point(int index)
{
	auto sequence = std::make_integer_sequence<int, puzzle_count>{};
	return get_puzzle_entry_points(sequence)[--index];
}

void run_puzzle(int index)
{
	std::vector<std::string> lines;

	std::string input_file_name = std::format("input/{:02}.txt", active_puzzle_index);

	std::ifstream file(input_file_name);
	std::string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	get_puzzle_entry_point(index)(lines);
}

int main()
{
	run_puzzle(active_puzzle_index);

	return 0;
}