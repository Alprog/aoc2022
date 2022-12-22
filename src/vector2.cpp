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

vector2 vector2::operator*(const int value)
{
	return { x * value, y * value };
}

vector2 vector2::operator/(const int value)
{
	return { x / value, y / value };
}

vector2& vector2::operator+=(const vector2& rhs)
{
	x += rhs.x;	y += rhs.y;	return *this;
}

vector2& vector2::operator-=(const vector2& rhs)
{
	x -= rhs.x;	y -= rhs.y;	return *this;
}

vector2& vector2::operator*=(const int value)
{
	x *= value; y *= value;	return *this;
}

vector2& vector2::operator/=(const int value)
{
	x /= value;	y /= value;	return *this;
}

int vector2::get_manhattan_size() const
{
	return std::abs(x) + std::abs(y);
}

vector2 vector2::min(vector2& lhs, vector2& rhs)
{
	return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
}

vector2 vector2::max(vector2& lhs, vector2& rhs)
{
	return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
}

std::strong_ordering vector2::operator<=>(const vector2& rhs) const
{
	return (x << 16) + y <=> (rhs.x << 16) + rhs.y;
}