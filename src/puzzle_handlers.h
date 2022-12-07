#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <functional>
#include <optional>

struct input
{
	std::vector<std::string> lines;
	int current_part;

	bool is_part_one() const;
	bool is_part_two() const;
};

struct output
{
	output() = default;
	output(const char* value);
	output(std::string value);

	template <typename T>
	output(T&& value)
		: result{ std::to_string(value) }
	{
	}

	std::optional<std::string> result;
};

using puzzle_handler = auto(*)(input&) -> output;

void register_puzzle_handler(int day, int part, puzzle_handler handler);
void try_run_puzzle_handler(int day, int part, input& input, output& output);
int get_max_puzzle_day();

constexpr int universal_part = 0;

template<int Day, int Part = 0>
struct puzzle
{
	template<typename T>
	puzzle(T handler)
	{
		register_puzzle_handler(Day, Part, handler);
	}
};

#define COMB(x, y) x ## y
#define COMB2(x, y) COMB(x, y)
#define X COMB2(x, __LINE__)