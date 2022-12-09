#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>

#include <set>

struct Vector2
{
	int x = 0;
	int y = 0;

	Vector2 operator+(const Vector2& rhs)
	{
		return { x + rhs.x, y + rhs.y };
	}

	Vector2 operator-(const Vector2& rhs)
	{
		return { x - rhs.x, y - rhs.y };
	}

	bool operator<(const Vector2& rhs) const
	{
		return (x << 16) + y < (rhs.x << 16) + rhs.y;
	}
};

Vector2 get_offset(char c)
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

int sgn(int val)
{
	if (val < 0)
		return -1;
	else if (val > 0)
		return 1;
	else
		return 0;
}

puzzle<9, 1> X = [](input& input) -> output
{
	Vector2 head, tail;

	std::set<Vector2> set;

	for (auto& line : input.lines)
	{
		auto offset = get_offset(line[0]);
		auto count = std::atoi(line.c_str() + 2);
		for (int i = 0; i < count; i++)
		{
			head = head + offset;

			auto dist = head - tail;
			if (std::abs(dist.x) >= 2 || std::abs(dist.y) >= 2)
			{
				tail.x += sgn(dist.x);
				tail.y += sgn(dist.y);
			}
			set.insert(tail);
		}
	}

	return set.size();
};

puzzle<9, 2> X = [](input& input) -> output
{
	constexpr int knot_count = 10;

	Vector2 knots[knot_count];
	auto& head = knots[0];
	auto& tail = knots[knot_count - 1];

	std::set<Vector2> set;

	for (auto& line : input.lines)
	{
		auto offset = get_offset(line[0]);
		auto count = std::atoi(line.c_str() + 2);
		
		for (int i = 0; i < count; i++)
		{
			head = head + offset;

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