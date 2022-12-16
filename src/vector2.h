#pragma once

#include <compare>

struct vector2
{
	int x = 0;
	int y = 0;

	vector2 operator+(const vector2& rhs);
	vector2 operator-(const vector2& rhs);

	int get_manhattan_size() const;

	static vector2 min(vector2& lhs, vector2& rhs);
	static vector2 max(vector2& lhs, vector2& rhs);

	std::strong_ordering operator<=>(const vector2& rhs) const;
};