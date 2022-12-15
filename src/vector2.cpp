#include "vector2.h"

#include <algorithm>

vector2 vector2::operator+(const vector2& rhs)
{
	return { x + rhs.x, y + rhs.y };
}

vector2 vector2::operator-(const vector2& rhs)
{
	return { x - rhs.x, y - rhs.y };
}

vector2 vector2::min(vector2& lhs, vector2& rhs)
{
	return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
}

vector2 vector2::max(vector2& lhs, vector2& rhs)
{
	return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
}

bool vector2::operator<(const vector2& rhs) const
{
	return (x << 16) + y < (rhs.x << 16) + rhs.y;
}