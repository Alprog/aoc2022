#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <functional>

struct input
{
	std::vector<std::string> lines;
	int current_part;

	bool is_part_one() { return current_part == 1; };
	bool is_part_two() { return current_part == 2; };
};

using puzzle_handler = void(*)(input&);

void register_puzzle_handler(int day, int part, puzzle_handler handler);
void try_run_puzzle_handler(int day, int part, input& input);
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