#include "puzzles.h"

#include "string_utils.h"

using stack = std::vector<char>;
using stacks = std::vector<std::vector<char>>;

void do_move(stack& src, stack& dst, int count)
{
	for (int i = 0; i < count; i++)
	{
		char c = src.back();
		src.pop_back();
		dst.emplace_back(c);
	}
}

void do_move_multiply(stack& src, stack& dst, int count)
{
	auto start = src.size() - count;
	dst.insert(dst.end(), src.begin() + start, src.end());
	src.resize(start);
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
		auto move_count = std::atoi(arr[1].c_str());
		auto from = std::atoi(arr[3].c_str()) - 1;
		auto to = std::atoi(arr[5].c_str()) - 1;

		auto& src = stacks[from];
		auto& dst = stacks[to];
		if (is_part_one)
		{
			do_move(src, dst, move_count);
		}
		else
		{
			do_move_multiply(src, dst, move_count);
		}		
	}


	std::string result;
	for (auto& stack : stacks)
	{
		result += stack.back();
	}
	std::cout << result << "\n";
}

//void puzzle<5, 1>::run(std::vector<std::string>& lines)
//{
//	solve(lines, true);
//}
//
//void puzzle<5, 2>::run(std::vector<std::string>& lines)
//{
//	solve(lines, false);
//}