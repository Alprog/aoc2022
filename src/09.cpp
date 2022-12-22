#include "puzzle_handlers.h"

#include "string_utils.h"
#include "vector2.h"
#include <functional>
#include <set>

int sgn(int val)
{
	if (val < 0)
		return -1;
	else if (val > 0)
		return 1;
	else
		return 0;
}

vector2 get_offset(char c)
{
	switch (c)
	{
		case 'R':
			return { 1, 0 };
		case 'L':
			return { -1, 0 };
		case 'D':
			return { 0, 1 };
		case 'U':
			return { 0, -1 };
	}
}

puzzle<9> X = [](input& input) -> output
{
	int knot_count = input.is_part_one() ? 2 : 10;

	std::vector<vector2> knots(knot_count);
	auto& head = knots.front();
	auto& tail = knots.back();

	std::set<vector2> set;

	for (auto& line : input.lines)
	{
		auto offset = get_offset(line[0]);
		auto count = std::atoi(line.c_str() + 2);
		
		for (int i = 0; i < count; i++)
		{
			head += offset;

			for (int j = 1; j < knot_count; j++)
			{
				auto dist = knots[j - 1] - knots[j];
				if (std::abs(dist.x) >= 2 || std::abs(dist.y) >= 2)
				{
					knots[j].x += sgn(dist.x);
					knots[j].y += sgn(dist.y);
				}
			}

			set.insert(tail);
		}
	}

	return set.size();
};