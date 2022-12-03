#include "puzzles.h"

#include "string_utils.h"

void puzzle<3, 1>::run(std::vector<std::string>& lines)
{
	auto total = 0;

	for (auto& line : lines)
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
}

void puzzle<3, 2>::run(std::vector<std::string>& lines)
{
	auto total = 0;

	auto index = 0;
	while (index < lines.size())
	{
		auto& line = lines[index++];
		auto& lineA = lines[index++];
		auto& lineB = lines[index++];

		auto size = line.size();
		for (int i = 0; i < size; i++)
		{
			char c = line[i];

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
}