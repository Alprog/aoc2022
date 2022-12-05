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

using puzzle_entry_point = void(*)(input&);

template<int Day, int Part = 0>
struct puzzle
{
	static void run(input& input)
	{
		auto& handler = get_handler();
		if (handler != nullptr)
		{
			handler(input);
		}
	}

	template<typename T>
	puzzle(T handler)
	{
		get_handler() = handler;
	}

	static std::function<void(input&)>& get_handler()
	{
		static std::function<void(input&)> handler;
		return handler;
	}	
};

#define COMB(x, y) x ## y
#define COMB2(x, y) COMB(x, y)
#define X COMB2(x, __LINE__)