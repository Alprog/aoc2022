#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>

struct vector2
{
	int x = 0;
	int y = 0;

	vector2 operator+(const vector2& rhs)
	{
		return { x + rhs.x, y + rhs.y };
	}

	vector2 operator-(const vector2& rhs)
	{
		return { x - rhs.x, y - rhs.y };
	}

	static vector2 min(vector2& lhs, vector2& rhs)
	{
		return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
	}

	static vector2 max(vector2& lhs, vector2& rhs)
	{
		return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
	}
};

bool add_sand(std::string& map, int index, int line_width, int height)
{
	if (map[index] != '.')
	{
		return false;
	}

	auto test = [&](int new_index) -> bool
	{
		if (map[new_index] == '.')
		{
			index = new_index;
			return true;
		}
		return false;
	};

	while (height > 0) 
	{
		auto base_index = index + line_width;
		if (test(base_index) || test(base_index - 1) || test(base_index + 1))
		{
			height--;
		}
		else
		{
			map[index] = '0';
			return true;
		}
	};

	return false;
}

puzzle<14> X = [](input& input) -> output
{
	std::vector<std::vector<vector2>> chains;

	vector2 min = { INT_MAX, 0 };
	vector2 max = { 0, 0 };

	for (auto& line : input.lines)
	{
		auto& chain = chains.emplace_back();

		auto pairs = str_utils::split(line, " -> ");
		for (auto& pair : pairs)
		{
			auto x = std::atoi(pair.c_str());
			auto y = std::atoi(pair.c_str() + pair.find_first_of(',') + 1);
			auto& position = chain.emplace_back(x, y);
			min = vector2::min(min, position);
			max = vector2::max(max, position);
		}
	}

	auto spawn_position = vector2(500, 0);
	if (input.is_part_one())
	{
		min.x--; // sentinels
		max.x++;
	}
	else
	{
		max.y += 2;

		auto height = max.y - min.y;
		min.x = std::min(min.x, spawn_position.x - height); // extend for full pyramid of sand
		max.x = std::max(max.x, spawn_position.x + height);
		chains.push_back({ {min.x, max.y }, {max.x, max.y } });
	}

	auto size = max - min + vector2(1, 1);

	int line_width = size.x + 1;
	
	int cell_count = line_width * size.y;

	std::string map;
	map.resize(cell_count, '.');
	for (int y = 0; y < size.y; y++)
	{
		map[line_width * y + size.x] = '\n';
	}
	
	for (auto& chain : chains)
	{
		for (int i = 0; i < chain.size() - 1; i++)
		{
			auto p1 = chain[i] - min;
			auto p2 = chain[i + 1] - min;

			auto min = vector2::min(p1, p2);
			auto max = vector2::max(p1, p2);
			for (auto x = min.x; x <= max.x; x++)
			{
				for (auto y = min.y; y <= max.y; y++)
				{
					map[line_width * y + x] = '#';
				}
			}
		}
	}

	auto new_spawn_position = spawn_position - min;
	auto spawn_index = new_spawn_position.y * line_width + new_spawn_position.x;

	int height = max.y - new_spawn_position.y;

	int step_index = 0;
	while (add_sand(map, spawn_index, line_width, height))
	{
		step_index++;
	}
	
	// Map visualization:
	// std::cout << std::endl << map;

	return step_index;
};