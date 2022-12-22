#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <map>
#include "vector2.h"

struct teleport
{
	int destination;
	int rotation;
};

struct monkey_map
{
	monkey_map(std::vector<std::string>& lines)
	{
		width = 0;
		for (auto& line : lines)
		{
			width = std::max(width, (int)line.size());
		}

		width += 3;
		height = lines.size() + 2;

		data.resize(width * height, ' ');
		
		int index = 1;
		for (auto& line : lines)
		{
			memcpy(&data[1] + width * index++, &line[0], line.size());
		}

		for (int i = 0; i < height; i++)
		{
			data[width * (i + 1) - 1] = '\n';
		}
	}

	int width;
	int height;
	std::string data;

	std::map<int, teleport> teleports;
};

struct cursor
{
	cursor(monkey_map& map)
		: map { map }
	{
		index = map.data.find_first_of('.');
		directions = { 1, map.width, -1, -map.width };
		direction_arrows = { '>', 'V', '<', '^' };
		step = directions[direction];
	}

	void move(int count)
	{
		for (int i = 0; i < count; i++)
		{
			int new_index = index + step;
			if (map.data[new_index] == ' ')
			{
				do 
				{
					new_index -= step;
				}
				while (map.data[new_index] != ' ');
				new_index += step;
			}
			if (map.data[new_index] == '#')
			{
				break;
			}
			else
			{
				draw_self();
				index = new_index;
			}
		}
	}

	void rotate(int offset)
	{
		int count = directions.size();
		direction = (direction + offset + count) % count;
		step = directions[direction];
	}

	void draw_self()
	{
		//map.data[index] = direction_arrows[direction];
	}

	int index;
	int direction = 0;
	monkey_map& map;
	
	std::vector<int> directions;
	std::vector<char> direction_arrows;
	int step;
};

puzzle<22, 1> X = [](input& input) -> output
{
	std::string sequence = input.lines.back();
	input.lines.resize(input.lines.size() - 2);

	monkey_map map(input.lines);
	cursor cursor(map);
	
	int value = 0;
	for (auto c : sequence)
	{
		if (c == 'R')
		{
			cursor.move(value);
			cursor.rotate(1);
			value = 0;
		}
		else if (c == 'L')
		{
			cursor.move(value);
			cursor.rotate(-1);
			value = 0;
		}
		else
		{
			value = value * 10 + (c - '0');
		}
	}
	cursor.move(value);

	std::cout << map.data;

	cursor.draw_self();

	auto x = cursor.index % map.width;
	auto y = cursor.index / map.width;

	return y * 1000 + x * 4 + cursor.direction;
};