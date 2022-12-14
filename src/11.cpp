#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include "binary_heap.h"

using int64 = int64_t;

struct monkey
{
	monkey(std::vector<monkey>& monkeys)
		: monkeys{ monkeys }
	{
	}

	std::vector<monkey>& monkeys;
	std::vector<int64> items;
	char operation;
	int64 operand;
	int64 test_division;
	int64 true_index;
	int64 false_index;
	int64 inspections = 0;

	void do_round(std::function<int64(int64)> callback)
	{
		inspections += items.size();
		for (auto& item : items)
		{
			switch (operation)
			{
			case '+':
				item += operand;
				break;
			case '*':
				item *= operand;
				break;
			case '^':
				item = std::pow(item, operand);
				break;
			}
			item = callback(item);
			int index = item % test_division == 0 ? true_index : false_index;
			monkeys[index].items.push_back(item);
		}

		items.clear();
	}
};

std::vector<monkey> fill_monkeys(std::vector<monkey>& monkeys, input& input)
{
	int index = 1;
	while (index < input.lines.size())
	{
		auto& monkey = monkeys.emplace_back(monkeys);

		auto& line = input.lines[index++];
		auto items_string = line.substr(line.find(':') + 2);
		auto items = str_utils::split(items_string, ", ");
		for (auto& item : items)
		{
			monkey.items.push_back(std::atoll(item.c_str()));
		}

		auto operator_arr = str_utils::split(input.lines[index++], " ");
		auto count = operator_arr.size();
		monkey.operation = operator_arr[count - 2][0];
		auto operand = operator_arr[count - 1];
		if (operand == "old" && monkey.operation == '*')
		{
			monkey.operation = '^';
			monkey.operand = 2;
		}
		else
		{
			monkey.operand = std::atoll(operand.c_str());
		}

		line = input.lines[index++];
		monkey.test_division = std::atoll(line.c_str() + line.find_last_of(' '));

		line = input.lines[index++];
		monkey.true_index = std::atoll(line.c_str() + line.find_last_of(' '));

		line = input.lines[index++];
		monkey.false_index = std::atoll(line.c_str() + line.find_last_of(' '));

		index += 2;
	}

	return monkeys;
}

puzzle<11> X = [](input& input) -> output
{
	std::vector<monkey> monkeys;
	fill_monkeys(monkeys, input);

	int64 multiplier = 1;
	for (auto& monkey : monkeys)
	{
		multiplier *= monkey.test_division;
	}

	std::function<int64(int64)> callbackA = [&](int64 value) { return value / 3; };
	std::function<int64(int64)> callbackB = [&](int64 value) { return value % multiplier; };
	auto callback = input.is_part_one() ? callbackA : callbackB;

	int count = input.is_part_one() ? 20 : 10'000;
	for (int round = 0; round < count; round++)
	{
		for (auto& monkey : monkeys)
		{
			monkey.do_round(callback);
		}
	}

	binary_heap_max<int64> heap;
	for (auto& monkey : monkeys)
	{
		heap.push(monkey.inspections);
	}

	return heap.pop() * heap.pop();
};