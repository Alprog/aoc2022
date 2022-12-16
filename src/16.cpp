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

	void make_fast_forward_link(node& neighbor, link& merged_link)
	{
		for (auto& link : links)
		{
			if (&link.target.get() == &neighbor)
			{
				link.target = merged_link.target;
				link.distance += merged_link.distance;
			}
		}
	}

	std::string name;
	int id;
	char label;
	float rate;
	std::vector<std::reference_wrapper<node>> neighbors;
	std::vector<int> distances;
	std::vector<link> links;
};

struct state
{
	int opened;
	int place;

	bool operator<(const state& rhs) const
	{
		if (opened < rhs.opened)
		{
			return true;
		}
		else if (opened > rhs.opened)
		{
			return false;
		}
		return place < rhs.place;
	}

	state get_moved_state(int new_place)
	{
		return { opened, new_place };
	}

	state get_opened_state()
	{
		return { opened | (1 << place), place };
	}

	bool is_opened()
	{
		return (opened & (1 << place)) != 0;
	}
};

struct path
{
	std::string way;
	int total_cost;
};

std::vector<std::reference_wrapper<node>> nodes;
std::map<std::string, std::reference_wrapper<node>> map;

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

puzzle<16, 1> X = [](input& input) -> output
{
	nodes.clear();
	map.clear();

	int id = 0;
	for (auto& line : input.lines)
	{
		std::string_view view = line;

		auto name = line.substr(6, 2);

		auto& node = get_node(name);
		node.id = id++;
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

	id = 0;
	for (node& cur : nodes)
	{
		cur.id = id++;
		cur.label = cur.id + 'A';
	}
	
	constexpr int max_steps = 31;

	state start_state{ 0, start_node.id };


	using state_map = std::map<state, path>;
	std::vector<state_map> states;
	states[0][start_state] = { "", 0 };

	states.resize(max_steps);



	return 0;
};