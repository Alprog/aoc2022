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

			if (std::find(line.begin() + half_size, line.end(), c) != line.end())
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

	for (int j = 0; j < lines.size(); j += 3)
	{
		auto line = lines[j];
		auto size = line.size();

		for (int i = 0; i < size; i++)
		{
			char c = line[i];

			auto lineA = lines[j + 1];
			auto lineB = lines[j + 2];

			if (std::find(lineA.begin(), lineA.end(), c) != lineA.end())
			{
				if (std::find(lineB.begin(), lineB.end(), c) != lineB.end())
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
	}

	std::cout << total << "\n";
}