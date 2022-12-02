#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

using puzzle_entry_point = void(*)(std::vector<std::string>&);

template<int Day, int Part>
class puzzle
{
public:
	static void run(std::vector<std::string>& lines);
};