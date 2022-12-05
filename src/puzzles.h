#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <functional>

using puzzle_entry_point = void(*)(std::vector<std::string>&);

struct input 
{
	std::vector<std::string> lines;
};

template<int Day, int Part = 0>
struct puzzle
{
	static void run(std::vector<std::string>& lines)
	{
		input input;
		input.lines = lines;

		auto& handler = get_handler();
		if (handler != nullptr)
		{
			handler(input);
		}
	}

	template<typename T>
	puzzle(T fn)
	{
		get_handler() = fn;
		std::cout << "!!";
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