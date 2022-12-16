#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <map>
#include <set>

struct node;

struct link
{
	std::reference_wrapper<node> target;
	int distance;
};

struct node
{
	node() {};

	node(std::string name)
		: name{name}
	{
	}

	std::string name;
	int id;
	float rate;
	std::vector<std::reference_wrapper<node>> neighbors;
	std::vector<int> distances;
	std::vector<link> links;
};

std::vector<std::reference_wrapper<node>> nodes;
std::map<std::string, std::reference_wrapper<node>> map;

struct cave_state
{
	int open_valves;
	int cur_place;

	auto operator<=>(const cave_state& rhs) const
	{
		auto result = open_valves <=> rhs.open_valves;
		if (result == std::strong_ordering::equal)
		{
			result = cur_place <=> rhs.cur_place;
		}
		return result;
	}

	cave_state get_moved_state(int new_place)
	{
		return { open_valves, new_place };
	}

	cave_state get_opened_state()
	{
		return { open_valves | (1 << cur_place), cur_place };
	}

	bool is_current_opened()
	{
		return (open_valves & (1 << cur_place)) != 0;
	}
};

struct short_state
{
	int open_valves;
	int total_gas;
};

struct full_state
{
	full_state() = default;

	full_state(cave_state cave, int step_left)
		: cave{ cave }
		, step_left{ step_left }
	{
	}

	cave_state cave;
	int gas_released = 0;
	int step_left = 0;	
	int gas_per_step = 0;

	void wait(int steps)
	{
		gas_released += gas_per_step * steps;
		step_left -= steps;
	}

	node& get_current_node()
	{
		return nodes[cave.cur_place];
	}

	int get_total_gas() 
	{
		return gas_released + gas_per_step * step_left;
	}

	short_state get_short_state()
	{
		return { cave.open_valves, get_total_gas() };
	}
};

using state_map = std::map<cave_state, full_state>;
std::vector<state_map> states;

node& get_node(std::string key)
{
	auto it = map.find(key);
	if (it != map.end())
	{
		return it->second;
	}

	auto new_node = new node(key);
	map.insert({ key, *new_node });
	return *new_node;
}

void clear_globals()
{
	for (auto& pair : map)
	{
		delete &pair.second.get();
	}
	nodes.clear();
	map.clear();
	states.clear();
}

void reset_ids()
{
	int id = 0;
	for (node& cur : nodes)
	{
		cur.id = id++;
	}
}

void parse_input(input& input)
{
	for (auto& line : input.lines)
	{
		std::string_view view = line;

		auto name = line.substr(6, 2);

		auto& node = get_node(name);
		nodes.push_back(node);
		node.rate = std::atoi(line.c_str() + line.find('=') + 1);

		auto index = view.find("valve");
		auto tail = view.substr(view.find(' ', index) + 1);
		auto arr = str_utils::split(tail, ", ");
		for (auto& neighbor_name : arr)
		{
			auto& neighbor = get_node(neighbor_name);
			node.neighbors.push_back(neighbor);
		}
	}

	reset_ids();
}

void simplify_graph()
{
	for (node& cur : nodes)
	{
		cur.distances.resize(nodes.size(), -1);
		cur.distances[cur.id] = 0;
		for (node& neighbor : cur.neighbors)
		{
			cur.distances[neighbor.id] = 1;
		}
	}

	bool changed = true;
	while (changed)
	{
		changed = false;
		for (node& cur : nodes)
		{
			for (int i = 0; i < nodes.size(); i++)
			{
				if (cur.distances[i] == -1)
				{
					for (node& neighbor : cur.neighbors)
					{
						if (neighbor.rate == 0 && neighbor.distances[i] != -1)
						{
							cur.distances[i] = neighbor.distances[i] + 1;
							changed = true;
							break;
						}
					}
				}
			}
		}
	}

	node& start_node = map.find("AA")->second;

	for (int i = nodes.size() - 1; i >= 0; i--)
	{
		node& cur = nodes[i];
		if (&cur != &start_node && cur.rate == 0)
		{
			nodes[i] = nodes.back();
			nodes.pop_back();
			for (node& n : nodes)
			{
				n.distances[i] = n.distances.back();
				n.distances.pop_back();
			}
		}
	}

	for (node& cur : nodes)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			if (cur.distances[i] > 0)
			{
				cur.links.emplace_back(nodes[i], cur.distances[i]);
			}
		}
	}

	reset_ids();
}

void fill_states(int max_steps)
{
	node& start_node = map.find("AA")->second;
	cave_state start_cave_state{ 0, start_node.id };
	full_state start_state(start_cave_state, max_steps);

	states.resize(max_steps + 1);

	auto try_set = [&](full_state& state)
	{
		if (state.step_left >= 0)
		{
			auto& map = states[state.step_left];
			auto it = map.find(state.cave);
			if (it != map.end())
			{
				if (it->second.get_total_gas() >= state.get_total_gas())
				{
					return;
				}
			}
			map[state.cave] = state;
		}
	};


	states.back()[start_cave_state] = start_state;

	for (auto step = max_steps; step > 0; step--)
	{
		for (auto& pair : states[step])
		{
			auto& state = pair.second;
			auto& cur_node = state.get_current_node();

			if (cur_node.rate > 0 && !state.cave.is_current_opened())
			{
				full_state new_state = state;
				new_state.cave = state.cave.get_opened_state();
				new_state.wait(1);
				new_state.gas_per_step += cur_node.rate;
				try_set(new_state);
			}

			for (auto& link : cur_node.links)
			{
				full_state new_state = state;
				new_state.cave = state.cave.get_moved_state(link.target.get().id);
				new_state.wait(link.distance);
				try_set(new_state);
			}
		}
	}
}

puzzle<16, 1> X = [](input& input) -> output
{
	parse_input(input);
	simplify_graph();
	fill_states(30);

	auto best_gas_released = 0;
	for (auto& step : states)
	{
		for (auto& pair : step)
		{
			best_gas_released = std::max(best_gas_released, pair.second.get_total_gas());
		}
	}

	clear_globals();
	return best_gas_released;
};
	
puzzle<16, 2> X = [](input& input)->output
{
	parse_input(input);
	simplify_graph();
	fill_states(26);

	std::vector<short_state> last_states;
	for (auto& state : states[0])
	{
		last_states.push_back(state.second.get_short_state());
	}

	auto best_gas_released = 0;
	for (int i = 0; i < last_states.size(); i++)
	{
		auto& a = last_states[i];
		for (int j = i + 1; j < last_states.size(); j++)
		{
			auto& b = last_states[j];
			if ((a.open_valves & b.open_valves) == 0)
			{
				auto gas_released = a.total_gas + b.total_gas;
				best_gas_released = std::max(best_gas_released, gas_released);
			}
		}
	}

	clear_globals();
	return best_gas_released;
};