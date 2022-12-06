#include "puzzle_handlers.h"
#include "binary_heap.h"

puzzle<1, 1> X = [](input& input)
{
	input.lines.push_back("");

	int max = 0;
	int cur = 0;
	for (auto& line : input.lines)
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

	std::cout << max << "\n";
};

puzzle<1, 2> X = [](input& input)
{
	binary_heap_max<int> heap;

	int cur = 0;
	for (auto& line : input.lines)
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

	auto result = heap.pop() + heap.pop() + heap.pop();
	std::cout << result << "\n";
};