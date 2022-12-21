#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <map>

struct monkey_op
{
	std::string name = "";
	std::string a = "";
	char operation = 0;
	std::string b = "";
};

int64_t operation(int64_t a, int64_t b, char operation)
{
	switch (operation)
	{
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '/':
			return a / b;
		case '*':
			return a * b;
		case '=':
			return a;
	}
}

puzzle<21> X = [](input& input) -> output
{
	std::map<std::string, int64_t> map;
	std::vector<monkey_op> monkeys;

	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, " ");
		auto name = arr[0].substr(0, arr[0].size() - 1);
		if (input.is_part_two() && name == "humn")
		{
			continue;
		}
		if (arr.size() == 2)
		{
			map[name] = std::atoi(arr[1].c_str());
		}
		else
		{
			auto& a = arr[1];
			auto& b = arr[3];
			auto& c = name;
			char operation = arr[2][0];
			if (input.is_part_two() && name == "root")
			{
				monkeys.emplace_back(a, b, '=', b);
				monkeys.emplace_back(b, a, '=', a);
				continue;
			}
			
			monkeys.emplace_back(c, a, operation, b);
			if (operation == '*')
			{
				monkeys.emplace_back(a, c, '/', b);
				monkeys.emplace_back(b, c, '/', a);
			}
			else if (operation == '/')
			{
				monkeys.emplace_back(a, c, '*', b);
				monkeys.emplace_back(b, a, '/', c);
			}
			else if (operation == '+')
			{
				monkeys.emplace_back(a, c, '-', b);
				monkeys.emplace_back(b, c, '-', a);
			}
			else if (operation == '-')
			{
				monkeys.emplace_back(a, c, '+', b);
				monkeys.emplace_back(b, a, '-', c);
			}
		}
	}

	while (!monkeys.empty())
	{
		for (int i = monkeys.size() - 1; i >= 0; i--)
		{
			auto monkey = monkeys[i];
			auto a = map.find(monkey.a);
			auto b = map.find(monkey.b);
			if (a != map.end() && b != map.end())
			{
				map[monkey.name] = operation(a->second, b->second, monkey.operation);
				monkeys[i] = monkeys.back();
				monkeys.pop_back();
			}
		}
	}

	return input.is_part_one() ? map["root"] : map["humn"];
};

