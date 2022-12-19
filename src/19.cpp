#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <algorithm>

struct resources
{
	resources(uint64_t all)
	{
		this->all = all;
	}

	resources(int ore, int clay, int obsidian, int geode)
	{
		res[0] = ore;
		res[1] = clay;
		res[2] = obsidian;
		res[3] = geode;
	}

	resources operator+(const resources& rhs)
	{
		return resources(all + rhs.all);
	}

	union
	{
		uint16_t res[4];
		uint64_t all;
	};
};

enum class order
{
	unordered = 0,
	less_or_equal = 1,
	greater = 2,
};

struct state
{
	int minutes_left = 0;
	resources balance = 0;
	resources per_minute = 0;

	void wait(int minutes = 1)
	{
		balance.all += per_minute.all * minutes;
		minutes_left -= minutes;
	}

	int get_wait_time(resources cost)
	{
		int time = 0;
		for (int i = 0; i < 4; i++)
		{
			int need = cost.res[i] - balance.res[i];
			if (need > 0)
			{
				int gain_rate = per_minute.res[i];
				if (gain_rate == 0)
				{
					return 100;
				}
				else
				{
					int need_time = need / gain_rate;
					if (need % gain_rate != 0)
					{
						need_time++;
					}
					time = std::max(time, need_time);
				}
			}
		}
		return time;
	}

	int get_geode_count()
	{
		return balance.res[3] + per_minute.res[3] * minutes_left;
	}

	std::string name;
};

order compare(state& a, state& b)
{
	bool less = false;
	bool greater = false;
	for (int i = 0; i < 4; i++)
	{
		less |= a.balance.res[i] < b.balance.res[i];
		less |= a.per_minute.res[i] < b.per_minute.res[i];
		greater |= a.balance.res[i] > b.balance.res[i];
		greater |= a.per_minute.res[i] > b.per_minute.res[i];
	}
	if (less && greater)
	{
		return order::unordered;
	}
	return greater ? order::greater : order::less_or_equal;
}

struct blueprint
{
	resources cost = 0;
	resources increase = 0;
};

std::vector<blueprint> blueprints;

void work(state state);

void try_work(state state, blueprint& blueprint)
{
	auto wait_time = state.get_wait_time(blueprint.cost) + 1;
	if (wait_time < state.minutes_left)
	{
		state.wait(wait_time);
		state.balance.all -= blueprint.cost.all;
		state.per_minute.all += blueprint.increase.all;
		work(state);
	}
}

int best_geode = 0;

void work(state state)
{
	for (auto& blueprint : blueprints)
	{
		try_work(state, blueprint);
	}

	int count = state.get_geode_count();
	best_geode = std::max(best_geode, count);
}

std::vector<std::vector<state>> steps;

void try_open(state state, blueprint& blueprint)
{
	auto wait_time = state.get_wait_time(blueprint.cost) + 1;
	if (wait_time < state.minutes_left)
	{
		state.wait(wait_time);
		state.balance.all -= blueprint.cost.all;
		state.per_minute.all += blueprint.increase.all;

		auto& step = steps[state.minutes_left];
		for (int i = step.size() - 1; i >= 0; i--)
		{
			auto cmp = compare(state, step[i]);
			if (cmp == order::less_or_equal)
			{
				return;
			}
			else if (cmp == order::greater)
			{
				step[i] = step.back();
				step.pop_back();
			}
		}
		step.push_back(state);
	}
}

void work2(state start_state) 
{
	steps.resize(start_state.minutes_left + 1);
	steps.back().push_back(start_state);

	for (int i = steps.size() - 1; i >= 0; i--)
	{
		auto& step = steps[i];
		if (step.size() > 0)
		{
			for (auto& state : step)
			{
				for (auto& blueprint : blueprints)
				{
					try_open(state, blueprint);
				}
			}
		}
	}
}

puzzle<19, 1> X = [](input& input) -> output
{
	steps.clear();

	blueprints.resize(4);
	blueprints[0].increase = { 1, 0, 0, 0 };
	blueprints[1].increase = { 0, 1, 0, 0 };
	blueprints[2].increase = { 0, 0, 1, 0 };
	blueprints[3].increase = { 0, 0, 0, 1 };

	int total = 0;
	int index = 0;
	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, " ");
		blueprints[0].cost = { std::atoi(arr[6].c_str()), 0, 0, 0};
		blueprints[1].cost = { std::atoi(arr[12].c_str()), 0, 0, 0 };
		blueprints[2].cost = { std::atoi(arr[18].c_str()), std::atoi(arr[21].c_str()), 0, 0 };
		blueprints[3].cost = { std::atoi(arr[27].c_str()), 0, std::atoi(arr[30].c_str()), 0 };

		best_geode = 0;
		state state;
		state.minutes_left = 32;
		state.per_minute.res[0] = 1;
		work2(state);

		std::cout << best_geode << std::endl;
	
		total += best_geode * ++index;
	}

	return total;
};

//puzzle<19, 2> X = [](input& input) -> output
//{
//	blueprints.resize(4);
//	blueprints[0].increase = { 1, 0, 0, 0 };
//	blueprints[1].increase = { 0, 1, 0, 0 };
//	blueprints[2].increase = { 0, 0, 1, 0 };
//	blueprints[3].increase = { 0, 0, 0, 1 };
//
//	std::vector<int> totals;
//	totals.resize(3, 1);
//	
//	int index = 0;
//	int line_count = input.lines.size();
//	line_count = std::max(line_count, 3);
//	for (int i = 0; i < line_count; i++)
//	{
//		auto arr = str_utils::split(input.lines[i], " ");
//		blueprints[0].cost = { std::atoi(arr[6].c_str()), 0, 0, 0 };
//		blueprints[1].cost = { std::atoi(arr[12].c_str()), 0, 0, 0 };
//		blueprints[2].cost = { std::atoi(arr[18].c_str()), std::atoi(arr[21].c_str()), 0, 0 };
//		blueprints[3].cost = { std::atoi(arr[27].c_str()), 0, std::atoi(arr[30].c_str()), 0 };
//
//		best_geode = 0;
//		state state;
//		state.minutes_left = 32;
//		state.per_minute.res[0] = 1;
//		work(state);
//
//		std::cout << best_geode << std::endl;
//
//		totals.push_back(best_geode);
//	}
//
//	return totals[0] * totals[1] * totals[2];
//};