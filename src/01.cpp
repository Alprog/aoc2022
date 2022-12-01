
#include "puzzles.h"
#include "binary_heap.h"

void puzzle<0>::run(std::vector<std::string>& lines)
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

void puzzle<1>::run(std::vector<std::string>& lines)
{
	lines.push_back("");

	binary_heap_max<int> heap;

	int cur = 0;
	for (auto& line : lines)
	{
		if (!line.empty())
		{
			cur += atoi(line.c_str());
		}
		else
		{
			heap.push(cur);
			cur = 0;
		}
	}

	std::cout << heap.pop() + heap.pop() + heap.pop();
}
