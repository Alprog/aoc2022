#include "puzzles.h"

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

			if (line.find(c, half_size) != std::string::npos)
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
		auto line = lines[index++];
		auto lineA = lines[index++];
		auto lineB = lines[index++];

		auto size = line.size();
		for (int i = 0; i < size; i++)
		{
			char c = line[i];

			if (lineA.find(c) != std::string::npos && lineB.find(c) != std::string::npos)
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