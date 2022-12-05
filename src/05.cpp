#include "puzzles.h"

#include "string_utils.h"

using stacks = std::vector<std::vector<char>>;

void do_move(stacks& stacks, int move, int from, int to)
{
	for (int i = 0; i < move; i++)
	{
		char c = stacks[from].back();
		stacks[from].pop_back();
		stacks[to].emplace_back(c);
	}
}

void do_move_multiply(stacks& stacks, int move, int from, int to)
{
	auto& stack_from = stacks[from];
	auto& stack_to = stacks[to];
	
	auto start = stack_from.size() - move;
	stack_to.insert(stack_to.end(), stack_from.begin() + start, stack_from.end());
	stack_from.resize(start);
}

void solve(std::vector<std::string>& lines, bool is_part_one)
{
	auto stack_count = (lines[0].size() + 1) / 4;

	auto empty_line_index = 0;
	while (!lines[empty_line_index].empty())
	{
		empty_line_index++;
	}

	stacks stacks;
	stacks.resize(stack_count);

	for (int index = empty_line_index - 2; index >= 0; index--)
	{
		auto line = lines[index];
		for (int i = 0; i < stack_count; i++)
		{
			char c = line[1 + 4 * i];
			if (c != ' ')
			{
				stacks[i].emplace_back(c);
			}
		}
	}

	for (int index = empty_line_index + 1; index < lines.size(); index++)
	{
		auto arr = str_utils::split(lines[index], " ");
		auto move = std::atoi(arr[1].c_str());
		auto from = std::atoi(arr[3].c_str()) - 1;
		auto to = std::atoi(arr[5].c_str()) - 1;
		if (is_part_one)
		{
			do_move(stacks, move, from, to);
		}
		else
		{
			do_move_multiply(stacks, move, from, to);
		}		
	}


	std::string result;
	for (int i = 0; i < stack_count; i++)
	{
		result += stacks[i].back();
	}
	std::cout << result << "\n";
}

void puzzle<5, 1>::run(std::vector<std::string>& lines)
{
	solve(lines, true);
}

void puzzle<5, 2>::run(std::vector<std::string>& lines)
{
	solve(lines, false);
}