#include "puzzle_handlers.h"

#include "string_utils.h"
#include "binary_heap.h"
#include "vector2.h"
#include <unordered_map>
#include <functional>

int lcm(int a, int b)
{
    int min = std::min(a, b);
    int max = std::max(a, b);
    int mult = max;
    while (mult % min != 0)
    {
        mult += max;
    }
    return mult;
}

struct blizzard_map
{
	int width;
	int height;
	int blizzard_width;
	int blizzard_height;
	int round_count;
	int totalsize;
	std::vector<char> data;
	input& puzzle_input;
	vector2 start_position;
	vector2 end_position;
	
	blizzard_map(input& input)
		: puzzle_input{ input }
	{
		width = input.lines.front().size();
		height = input.lines.size();

		blizzard_width = width - 2;
		blizzard_height = height - 2;

		round_count = lcm(blizzard_width, blizzard_height);
		totalsize = width * height * round_count;

		data.resize(totalsize, '#');

		start_position = vector2(1, 0);
		end_position = vector2(width - 2, height - 1);

		for (int round = 0; round < round_count; round++)
		{
			for (int y = 1; y < height - 1; y++)
			{
				for (int x = 1; x < width - 1; x++)
				{
					if (calc_is_inner_free(x, y, round))
					{
						data[to_index(x, y, round)] = '.';
					}
				}
			}

			data[to_index(start_position.x, start_position.y, round)] = '.';
			data[to_index(end_position.x, end_position.y, round)] = '.';
		}
	}

	int to_index(int x, int y, int round)
	{
		return (round * height + y) * width + x;
	}

	char sample_blizzard(int x, int y)
	{
		x = (x - 1 + round_count) % blizzard_width + 1;
		y = (y - 1 + round_count) % blizzard_height + 1;
		return puzzle_input.lines[y][x];
	}

	bool calc_is_inner_free(uint8_t x, uint8_t y, uint16_t round)
	{
		if (sample_blizzard(x - round, y) == '>')
			return false;

		if (sample_blizzard(x + round, y) == '<')
			return false;

		if (sample_blizzard(x, y - round) == 'v')
			return false;

		if (sample_blizzard(x, y + round) == '^')
			return false;

		return true;
	}

	bool is_free(int index)
	{
		if (index < 0 || index >= totalsize)
		{
			return false;
		}
		return data[index] == '.';
	}
};

struct game;
struct node_storage;

struct node
{
	game& game;
	int x;
	int y;
	int round;
	int cost;
	int priority;
	bool expired = false;

	node(::game& game, int x, int y, int round, int cost);
	int get_index();
	void try_open(int x, int y, int round);
	void open_all();
};

struct comparator
{
	bool operator()(const std::shared_ptr<node> lhs, const std::shared_ptr<node> rhs) const
	{
		return lhs->priority > rhs->priority;
	}
};

struct game
{
	blizzard_map& map;
	std::unordered_map<int, std::shared_ptr<node>> nodes;
	binary_heap<std::shared_ptr<node>, comparator> heap;

	void clear()
	{
		nodes.clear();
		heap.data.clear();
	}
};

node::node(::game& game, int x, int y, int round, int cost)
	: game{ game }
	, x{ x }
	, y{ y }
	, round{ round }
	, cost{ cost }
	, expired{ false }
{
	auto delta = game.map.end_position - vector2(x, y);
	priority = cost + std::abs(delta.x) + std::abs(delta.y);
}

int node::get_index()
{
	return game.map.to_index(x, y, round);
}

void node::open_all()
{
	auto new_round = (round + 1) % game.map.round_count;
	try_open(x, y, new_round);
	try_open(x + 1, y, new_round);
	try_open(x - 1, y, new_round);
	try_open(x, y + 1, new_round);
	try_open(x, y - 1, new_round);
}

void node::try_open(int x, int y, int round)
{
	int index = game.map.to_index(x, y, round);
	if (game.map.is_free(index))
	{
		auto it = game.nodes.find(index);
		if (it != game.nodes.end())
		{
			if (it->second->cost <= cost + 1)
			{
				return; // already contains better one
			}
			else
			{
				it->second->expired = true;
			}
		}

		auto new_node = std::make_shared<node>(game, x, y, round, cost + 1);
		game.nodes[index] = new_node;
		game.heap.push(new_node);
	}
}

int solve_trip(game& game, int start_round)
{
	start_round = start_round % game.map.round_count;

	auto pos = game.map.start_position;
	auto new_node = std::make_shared<node>(game, pos.x, pos.y, start_round, 0);
	game.nodes[new_node->get_index()] = new_node;
	game.heap.push(new_node);

	while (true)
	{
		auto node = game.heap.pop();
		if (node->priority - node->cost == 0)
		{
			return node->cost;
		}

		if (!node->expired)
		{
			node->open_all();
		}
	}
}

puzzle<24> X = [](input& input) -> output
{
	blizzard_map map(input);
	game game{ map };

	int total_count = solve_trip(game, 0);
	if (input.is_part_two())
	{
		for (int i = 0; i < 2; i++)
		{
			game.clear();
			std::swap(game.map.start_position, game.map.end_position);
			total_count += solve_trip(game, total_count);
		}
	}

	return total_count;
};