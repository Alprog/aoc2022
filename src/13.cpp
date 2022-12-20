#include <algorithm>

#include "puzzle_handlers.h"

#include "string_utils.h"
#include "auto_cleanup_vector.h"
#include <functional>

struct unit
{
	virtual ~unit() = default;

	virtual std::string to_string() = 0;
	virtual int size() = 0;
	virtual unit* get_child(int index) = 0;
	virtual int get_left_value() = 0;
	virtual bool is_single_value() { return false; };
};

struct item_list : public unit
{
	auto_cleanup_vector<unit> items;

	virtual int size() override { return items.size(); };
	virtual unit* get_child(int i) override { return items[i]; };
	virtual int get_left_value() override { return !items.empty() ? items[0]->get_left_value() : -1; };

	virtual std::string to_string() override
	{
		std::string result = "[";
		for (int i = 0; i < items.size(); i++)
		{
			if (i > 0)
			{
				result += ",";
			}
			result += items[i]->to_string();
		}
		result += "]";
		return result;
	};
};

struct single_item : public unit
{
	single_item(int value) 
		: value{ value }
	{
	}

	int value;

	virtual int size() override { return 1; };
	virtual unit* get_child(int i) override { return this; };
	virtual int get_left_value() override { return value; };
	virtual bool is_single_value() override { return true; };

	virtual std::string to_string() override { return std::to_string(value); };
};

unit* parse_unit(const char*& ptr);

single_item* parse_value(const char*& it)
{
	int value = 0;
	
	while (true)
	{
		auto c = *it;
		if (c == ',' || c == ']')
		{
			return new single_item{ value };
		}
		else
		{
			value = value * 10 + c - '0';
			it++;
		}
	}
}

item_list* parse_list(const char*& it)
{
	auto list = new item_list();
	if (it[1] == ']')
	{
		it += 2;
		return list;
	}

	while (*it++ != ']')
	{
		list->items.push_back(parse_unit(it));
	}
	return list;
}

unit* parse_unit(const char*& it)
{
	if (*it == '[')
	{
		return parse_list(it);
	}
	else
	{
		return parse_value(it);
	}
}

unit* parse(std::string_view line)
{
	auto it = &line[0];
	return parse_unit(it);
}

void print_pair(unit& unitA, unit& unitB)
{
	std::cout << unitA.to_string() << std::endl;
	std::cout << unitB.to_string() << std::endl;
	std::cout << std::endl;
}

int compare(unit& lhs, unit& rhs)
{
	int lhs_size = lhs.size();
	int rhs_size = rhs.size();

	int min_count = std::min(lhs_size, rhs_size);
	if (min_count > 0)
	{
		if (lhs.is_single_value() || rhs.is_single_value())
		{
			auto result = rhs.get_left_value() - lhs.get_left_value();
			if (result != 0)
			{
				return result;
			}
		}
		else
		{
			for (int i = 0; i < min_count; i++)
			{
				auto result = compare(*lhs.get_child(i), *rhs.get_child(i));
				if (result != 0)
				{
					return result;
				}
			}
		}
	}

	return rhs_size - lhs_size;
}

puzzle<13, 1> X = [](input& input) -> output
{
	int total = 0;

	int pair_count = (input.lines.size() + 1) / 3;
	for (int index = 0; index < pair_count; index++)
	{
		auto& lineA = input.lines[index * 3 + 0];
		auto& lineB = input.lines[index * 3 + 1];
		auto& unitA = *parse(lineA);
		auto& unitB = *parse(lineB);

		if (unitA.to_string() != lineA || unitB.to_string() != lineB)
		{
			break;
		}

		if (compare(unitA, unitB) > 0)
		{
			total += (index + 1);
		}
	}

	return total;
};

puzzle<13, 2> X = [](input& input) -> output
{
	auto_cleanup_vector<unit> units;
	for (auto& line : input.lines)
	{
		if (!line.empty())
		{
			units.push_back(parse(line));
		}
	}

	auto* unitA = parse("[[2]]");
	auto* unitB = parse("[[6]]");
	units.push_back(unitA);
	units.push_back(unitB);

	std::sort(units.begin(), units.end(), [](unit*& lhs, unit*& rhs) { return compare(*lhs, *rhs) > 0; });

	auto indexA = std::find(units.begin(), units.end(), unitA) - units.begin() + 1;
	auto indexB = std::find(units.begin(), units.end(), unitB) - units.begin() + 1;

	return indexA * indexB;
};