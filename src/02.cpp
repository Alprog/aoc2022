#include "puzzles.h"

puzzle<2, 1> X = [](input& input)
{
	auto score = 0;
	for (auto& line : input.lines)
	{
		auto enemy = line[0] - 'A';
		auto my = line[2] - 'X';

		auto result = (my - enemy + 4) % 3;

		score += (my + 1) + (result * 3);

	}
	std::cout << score << "\n";
};

puzzle<2, 1> X = [](input& input)
{
	auto score = 0;
	for (auto& line : input.lines)
	{
		auto enemy = line[0] - 'A';
		auto result = line[2] - 'X';

		auto my = (result + enemy + 2) % 3;

		score += (my + 1) + (result * 3);

	}
	std::cout << score << "\n";
};