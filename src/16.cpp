#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <map>

struct node
{
	node() {};

	node(std::string name)
		: name{name}
	{
	}

	std::string name;
	int id;
	char label;
	float rate;
	std::vector<std::reference_wrapper<node>> neighbors;
	std::vector<int> distances;
};

struct path
{
	std::string name;
	int total_cost;
	int total_rate;
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
	
	while (true)
	{
		int holes = 0;

		for (node& cur : nodes)
		{
			for (int i = 0; i < nodes.size(); i++)
			{
				if (cur.distances[i] == -1)
				{
					holes++;
					for (node& neighbor : cur.neighbors)
					{
						if (neighbor.distances[i] != -1)
						{
							cur.distances[i] = neighbor.distances[i] + 1;
							holes--;
							break;
						}
					}
				}
			}
		}

		if (holes == 0)
		{
			break;
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

	char label = 'A';
	for (node& cur : nodes)
	{
		cur.label = label++;
	}
	
	std::map<std::string, path> pathes;



	return 0;
};