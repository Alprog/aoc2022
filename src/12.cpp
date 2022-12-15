#include "puzzle_handlers.h"

#include "string_utils.h"
#include "vector2.h"
#include <functional>

struct node
{
	node(int x, int y, char height)
		: position{x, y}
		, height {height}
		, travel_price { INT_MAX }
	{
	}

	vector2 position;
	char height;
	int travel_price;
};

puzzle<12> X = [](input& input) -> output
{
	auto width = input.lines[0].size();
	auto height = input.lines.size();

	node* p_start_node = nullptr;
	node* p_end_node = nullptr;

	std::vector<std::vector<node>> map;
	map.reserve(height);
	for (int y = 0; y < height; y++)
	{
		auto& row = map.emplace_back();
		row.reserve(width);

		for (int x = 0; x < width; x++)
		{
			auto height = input.lines[y][x];
			auto& node = row.emplace_back(x, y, height);
			if (node.height == 'S')
			{
				p_start_node = &node;
				node.height = 'a';
			}
			else if (node.height == 'E')
			{
				p_end_node = &node;
				node.height = 'z';
			}
		}
	}

	auto& start_node = *p_start_node;
	auto& end_node = *p_end_node;

	end_node.travel_price = 0;

	std::vector<std::reference_wrapper<node>> wave{ end_node };
	std::vector<std::reference_wrapper<node>> next_wave;

	auto check = [&](node& node, int dx, int dy)
	{
		auto x = node.position.x + dx;
		auto y = node.position.y + dy;
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			auto& neighbor = map[y][x];
			if (neighbor.travel_price == INT_MAX)
			{
				if (neighbor.height + 1 >= node.height)
				{
					neighbor.travel_price = node.travel_price + 1;
					next_wave.push_back(neighbor);
				}
			}
		}
	};

	while (!wave.empty())
	{
		for (auto& node : wave)
		{
			check(node, 1, 0);
			check(node, -1, 0);
			check(node, 0, 1);
			check(node, 0, -1);
		}

		std::swap(wave, next_wave);
		next_wave.clear();
	}

	if (input.is_part_one())
	{
		return start_node.travel_price;
	}
	else
	{
		int best_price = INT_MAX;
		for (auto& row : map)
		{
			for (auto& node : row)
			{
				if (node.height == 'a' && node.travel_price < best_price)
				{
					best_price = node.travel_price;
				}
			}
		}

		return best_price;
	}
};