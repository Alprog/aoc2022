
#include "puzzles.h"

void puzzle<1>::run(std::vector<std::string>& lines)
{
	lines.push_back("");

	int max = 0;
	int cur = 0;
	for (auto& line : lines)
	{
		if (!line.empty())
		{
			cur += atoi(line.c_str());
		}
		else
		{
			max = std::max(cur, max);
			cur = 0;
		}
	}

	std::cout << max;
}
