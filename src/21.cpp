#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <map>

struct monkey_op
{
	std::string name = "";
	std::string a = "";
	std::string b = "";
	char operation = 0;
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
	}
}

puzzle<21, 1> X = [](input& input) -> output
{
	std::map<std::string, int64_t> map;
	std::vector<monkey_op> monkeys;

	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, " ");
		auto name = arr[0].substr(0, arr[0].size() - 1);
		if (arr.size() == 2)
		{
			map[name] = std::atoi(arr[1].c_str());
		}
		else
		{
			monkeys.emplace_back( name, arr[1], arr[3], arr[2][0] );
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

	int64_t root = map["root"];

	return root;
};