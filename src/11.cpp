#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include "binary_heap.h"

struct monkey
{
	monkey(std::vector<monkey>& monkeys, std::vector<long long> items, std::function<long long(long long)> operation, long long test_division, long long true_index, long long false_index)
		: monkeys{ monkeys }
		, items {items}
		, operation{ operation }
		, test_division{ test_division }
		, true_index{ true_index }
		, false_index{ false_index }
	{
	}

	std::vector<monkey>& monkeys;
	std::vector<long long> items;
	std::function<long long(long long)> operation;
	long long test_division;
	long long true_index;
	long long false_index;
	long long inspections = 0;

	void do_round()
	{
		inspections += items.size();
		for (auto& item : items)
		{
			item = operation(item);
			//item /= 3;
			int index = item % test_division == 0 ? true_index : false_index;
			monkeys[index].items.push_back(item);
		}

		items.clear();
	}

	void clip(long long multiplier)
	{
		for (auto& item : items)
		{
			while (item > multiplier)
			{
				item %= multiplier;
			}
		}
	}
};

puzzle<11, 1> X = [](input& input) -> output
{
	std::vector<monkey> monkeys;

	//monkeys.emplace_back(monkeys, std::vector<long long>{ 79, 98 }, [](long long value) { return value * 19; }, 23, 2, 3);
	//monkeys.emplace_back(monkeys, std::vector<long long>{ 54, 65, 75, 74 }, [](long long value) { return value + 6; }, 19, 2, 0);
	//monkeys.emplace_back(monkeys, std::vector<long long>{ 79, 60, 97 }, [](long long value) { return value * value; }, 13, 1, 3);
	//monkeys.emplace_back(monkeys, std::vector<long long>{ 74 }, [](long long value) { return value + 3; }, 17, 0, 1);

	monkeys.emplace_back(monkeys, std::vector<long long>{ 99, 63, 76, 93, 54, 73 }, [](long long value) { return value * 11; }, 2, 7, 1);
	monkeys.emplace_back(monkeys, std::vector<long long>{ 91, 60, 97, 54 }, [](long long value) { return value + 1; }, 17, 3, 2);
	monkeys.emplace_back(monkeys, std::vector<long long>{ 65 }, [](long long value) { return value + 7; }, 7, 6, 5);
	monkeys.emplace_back(monkeys, std::vector<long long>{ 84, 55 }, [](long long value) { return value + 3; }, 11, 2, 6);

	monkeys.emplace_back(monkeys, std::vector<long long>{ 86, 63, 79, 54, 83 }, [](long long value) { return value * value; }, 19, 7, 0);
	monkeys.emplace_back(monkeys, std::vector<long long>{ 96, 67, 56, 95, 64, 69, 96 }, [](long long value) { return value + 4; }, 5, 4, 0);
	monkeys.emplace_back(monkeys, std::vector<long long>{ 66, 94, 70, 93, 72, 67, 88, 51 }, [](long long value) { return value * 5; }, 13, 4, 5);
	monkeys.emplace_back(monkeys, std::vector<long long>{ 59, 59, 74 }, [](long long value) { return value + 8; }, 3, 1, 3);

	long long multiplier = 1;
	for (auto& monkey : monkeys)
	{
		multiplier *= monkey.test_division;
	}

	for (int round = 0; round < 10000; round++)
	{
		for (auto& monkey : monkeys)
		{
			monkey.clip(multiplier);
			monkey.do_round();
		}
	}

	binary_heap_max<long long> heap;
	for (auto& monkey : monkeys)
	{
		heap.push(monkey.inspections);
	}

	return heap.pop() + heap.pop();
};
