#include "puzzle_handlers.h"

bool input::is_part_one() const { return current_part == 1; };
bool input::is_part_two() const { return current_part == 2; };

output::output(const char* value) : result{ value } {}
output::output(std::string value) : result{ value } {}

using handler_table = std::vector<std::vector<puzzle_handler>>;

handler_table& get_handler_table()
{
	static handler_table table;
	return table;
}

void register_puzzle_handler(int day, int part, puzzle_handler handler)
{
	auto& table = get_handler_table();
	if (table.size() <= day)
	{
		table.resize(day + 1);
	}
	if (table[day].size() <= part)
	{
		table[day].resize(part + 1);
	}
	table[day][part] = handler;
}

void try_run_puzzle_handler(int day, int part, input& input, output& output)
{
	auto& table = get_handler_table();
	if (table.size() > day && table[day].size() > part)
	{
		auto& handler = table[day][part];
		if (handler != nullptr)
		{
			if (!output.result.has_value())
			{
				output = handler(input);
			}
		}
	}
}

int get_max_puzzle_day()
{
	return get_handler_table().size() - 1;
}