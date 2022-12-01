
#include "puzzles.h"

#include <fstream>
#include <format>
#include <type_traits>

constexpr int puzzle_count = 1;
constexpr int part_count = 2;

constexpr int active_puzzle_index = 1;

template <int>
struct day_index {};

template<int Day, int... Parts>
auto& get_part_functions(day_index<Day>, std::integer_sequence<int, Parts...> part_sequence)
{
	static std::vector<puzzle_entry_point> part_functions = { puzzle<Day + 1, Parts + 1>::run... };
	return part_functions;
}

template<int... Days>
auto& get_puzzle_functions(std::integer_sequence<int, Days...> day_sequence)
{
	constexpr auto part_sequence = std::make_integer_sequence<int, part_count>{};
	static std::vector<puzzle_entry_point*> puzzle_functions = { &get_part_functions(day_index<Days>{}, part_sequence)[0]...};
	return puzzle_functions;
}

puzzle_entry_point get_puzzle_entry_point(int day, int part)
{
	constexpr auto day_sequence = std::make_integer_sequence<int, puzzle_count>{};
	return get_puzzle_functions(day_sequence)[day - 1][part - 1];
}

void run_puzzle(int day)
{
	std::vector<std::string> lines;

	std::string input_file_name = std::format("input/{:02}.txt", active_puzzle_index);

	std::ifstream file(input_file_name);
	std::string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	for (int part = 1; part <= part_count; part++)
	{
		get_puzzle_entry_point(day, part)(lines);
	}
}

int main()
{
	run_puzzle(active_puzzle_index);

	return 0;
}