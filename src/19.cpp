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

	union
	{
		uint16_t res[4];
		uint64_t all;
	};
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

struct blueprint
{
	resources cost = 0;
	resources increase = 0;
};

int best_geode_count = 0;
std::vector<blueprint> blueprints;
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
		step.push_back(state);
	}
}

void clear_step(std::vector<state>& step)
{
	uint16_t best_g = 0;
	for (auto& s : step)
	{
		best_g = std::max(best_g, s.per_minute.res[3]);
	}

	for (int i = step.size() - 1; i >= 0; i--)
	{
		if (step[i].per_minute.res[3] < best_g)
		{
			step[i] = step.back();
			step.pop_back();
		}
	}
}

void work(state start_state) 
{
	steps.clear();
	steps.resize(start_state.minutes_left + 1);
	for (auto& step : steps)
	{
		step.reserve(500'000);
	}
	steps.back().push_back(start_state);

	for (int i = steps.size() - 1; i >= 0; i--)
	{
		auto& step = steps[i];

		clear_step(step);

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

	for (auto& step : steps)
	{
		for (auto& s : step)
		{
			best_geode_count = std::max(best_geode_count, s.get_geode_count());
		}
	}
}

void init_blueprints()
{
	blueprints.resize(4);
	blueprints[0].increase = { 1, 0, 0, 0 };
	blueprints[1].increase = { 0, 1, 0, 0 };
	blueprints[2].increase = { 0, 0, 1, 0 };
	blueprints[3].increase = { 0, 0, 0, 1 };
}

void parse_line(std::string& line)
{
	auto arr = str_utils::split(line, " ");
	blueprints[0].cost = { std::atoi(arr[6].c_str()), 0, 0, 0 };
	blueprints[1].cost = { std::atoi(arr[12].c_str()), 0, 0, 0 };
	blueprints[2].cost = { std::atoi(arr[18].c_str()), std::atoi(arr[21].c_str()), 0, 0 };
	blueprints[3].cost = { std::atoi(arr[27].c_str()), 0, std::atoi(arr[30].c_str()), 0 };
}

puzzle<19, 1> X = [](input& input) -> output
{
	init_blueprints();

	int total = 0;
	int index = 0;
	for (auto& line : input.lines)
	{
		parse_line(line);

		best_geode_count = 0;
		state state;
		state.minutes_left = 24;
		state.per_minute.res[0] = 1;
		work(state);

		total += best_geode_count * ++index;
	}

	return total;
};

puzzle<19, 2> X = [](input& input) -> output
{
	init_blueprints();

	std::vector<int> totals = { 1, 1, 1 };
	
	int line_count = input.lines.size();
	line_count = std::min(line_count, 3);
	for (int i = 0; i < line_count; i++)
	{
		parse_line(input.lines[i]);

		best_geode_count = 0;
		state state;
		state.minutes_left = 32;
		state.per_minute.res[0] = 1;
		work(state);

		std::cout << best_geode_count << " " << std::endl;

		totals[i] = best_geode_count;
	}

	return totals[0] * totals[1] * totals[2];
};