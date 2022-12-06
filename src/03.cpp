#include "puzzle_handlers.h"

#include "string_utils.h"

puzzle<3, 1> X = [](input& input)
{
	auto total = 0;

	for (auto& line : input.lines)
	{
		auto size = line.size();
		auto half_size = size / 2;

		for (int i = 0; i < half_size; i++)
		{
			char c = line[i];
			if (str_utils::contains(line, c, half_size))
			{
				if (c >= 'A' && c <= 'Z')
				{
					total += c - 'A' + 27;
				}
				else
				{
					total += c - 'a' + 1;
				}

				break;
			}

		}

	}

	std::cout << total << "\n";
};

puzzle<3, 2> X = [](input& input)
{
	auto total = 0;

	auto index = 0;
	while (index < input.lines.size())
	{
		auto& line = input.lines[index++];
		auto& lineA = input.lines[index++];
		auto& lineB = input.lines[index++];

		auto size = line.size();
		for (char c : line)
		{
			if (str_utils::contains(lineA, c) && str_utils::contains(lineB, c))
			{
				if (c >= 'A' && c <= 'Z')
				{
					total += c - 'A' + 27;
				}
				else
				{
					total += c - 'a' + 1;
				}

				break;
			}

		}
	}

	std::cout << total << "\n";
};