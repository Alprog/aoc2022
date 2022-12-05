#include "puzzles.h"

#include "string_utils.h"

using stacks = std::vector<std::vector<char>>;

void do_move(stacks& stacks, int move, int from, int to)
{
	for (int i = 0; i < move; i++)
	{
		char c = stacks[from].at(stacks[from].size() - 1);
		stacks[from].pop_back();
		stacks[to].emplace_back(c);
	}
}

void do_move_multiply(stacks& stacks, int move, int from, int to)
{
	auto& stack_from = stacks[from];
	auto& stack_to = stacks[to];

	auto start = stack_from.size() - move;
	for (int i = 0; i < move; i++)
	{
		stack_to.emplace_back(stack_from[start + i]);
	}
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

	for (int i = empty_line_index - 2; i >= 0; i--)
	{
		auto line = lines[i];
		for (int j = 0; j < stack_count; j++)
		{
			char c = line[1 + 4 * j];
			if (c != ' ')
			{
				stacks[j].emplace_back(c);
			}
		}
	}

	for (int i = empty_line_index + 1; i < lines.size(); i++)
	{
		auto arr = str_utils::split(lines[i], " ");
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
	for (int j = 0; j < stack_count; j++)
	{
		result += stacks[j][stacks[j].size() - 1];
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