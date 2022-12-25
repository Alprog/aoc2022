#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>

int64_t to_decimal(char c)
{
	switch (c)
	{
	case '=':
		return -2;
	case '-':
		return -1;
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	}
}

char to_snafu_digit(int64_t value)
{
	constexpr char snafu_digits[] = "=-012";
	return snafu_digits[value + 2];
}

int64_t to_decimal(std::string line)
{
	int64_t total = 0;

	int64_t count = line.size();
	int64_t base = 1;
	for (int64_t i = count - 1; i >= 0; i--)
	{
		auto value = to_decimal(line[i]) * base;
		total += value;
		base *= 5;
	}

	return total;
}

std::string invert(std::string number)
{
	for (auto& c : number)
	{
		c = to_snafu_digit(-to_decimal(c));
	}
	return number;
}

std::string to_snafu(int64_t value)
{
	if (value < 0)
	{
		return invert(to_snafu(-value));
	}

	std::string result;
	while (value != 0)
	{
		int64_t d = value % 5;
		if (d > 2) d -= 5;		
		result = to_snafu_digit(d) + result;
		value -= d;
		value /= 5;
	}
	return result;
}

puzzle<25, 1> X = [](input& input) -> output
{
	int64_t total = 0;

	for (auto& line : input.lines)
	{
		total += to_decimal(line);
	}

	return to_snafu(total);
};