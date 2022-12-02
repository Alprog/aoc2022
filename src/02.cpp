#include "puzzles.h"

void puzzle<2, 1>::run(std::vector<std::string>& lines)
{
	auto score = 0;
	for (auto& line : lines)
	{
		auto enemy = line[0] - 'A';
		auto my = line[2] - 'X';

		auto result = (my - enemy + 4) % 3;

		score += (my + 1) + (result * 3);

	}
	std::cout << score << "\n";
}

void puzzle<2, 2>::run(std::vector<std::string>& lines)
{
	auto score = 0;
	for (auto& line : lines)
	{
		auto enemy = line[0] - 'A';
		auto result = line[2] - 'X';

		auto my = (result + enemy + 2) % 3;

		score += (my + 1) + (result * 3);

	}
	std::cout << score << "\n";
}