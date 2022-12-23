#include "puzzle_handlers.h"

#include "string_utils.h"
#include "vector2.h"
#include <functional>

int const east = 0;
int const south = 1;
int const west = 2;
int const north = 3;

int const direction_count = 4;

int wrap(int direction);
int invert(int direction);

struct movement
{
	int primary;
	int secondaryA;
	int secondaryB;
};

struct grove_map
{
	int width;
	int height;
	std::string data;
	std::vector<int> direction_steps;
	std::vector<int> all_direction_steps;
	std::vector<movement> movements;

	grove_map(std::vector<std::string>& lines, int border)
	{
		width = lines[0].size();
		height = lines.size();

		width += border * 2;
		height += border * 2;

		width++; // for /n

		data.resize(width * height, '.');

		int index = border;
		for (auto& line : lines)
		{
			memcpy(&data[border] + width * index++, &line[0], line.size());
		}

		for (int i = 0; i < height; i++)
		{
			data[width * (i + 1) - 1] = '\n';
		}

		direction_steps = { 1, width, -1, -width };
		for (int i = 0; i < direction_count; i++)
		{
			all_direction_steps.push_back(direction_steps[i]);
			all_direction_steps.push_back(direction_steps[i] + direction_steps[(i + 1) % direction_count]);
		}

		std::vector<int> directions_order = { north, south, west, east };
		for (int main_dir : directions_order)
		{
			auto dir_a = wrap(main_dir - 1);
			auto dir_b = wrap(main_dir + 1);
			int primary = direction_steps[main_dir];
			int secondaryA = primary + direction_steps[dir_a];
			int secondaryB = primary + direction_steps[dir_b];
			movements.emplace_back(primary, secondaryA, secondaryB);
		}
	}

	bool is_free(int location, movement& m)
	{
		return data[location + m.primary] != '#' && data[location + m.secondaryA] != '#' && data[location + m.secondaryB] != '#';
	}

	int propose(int location, movement& m)
	{
		int new_location = location + m.primary;
		data[new_location] = data[new_location] == '.' ? '1' : 'X';
		return new_location;
	}

	int to_index(vector2 position)
	{
		return position.x + position.y * width;
	}

	vector2 to_position(int index)
	{
		return { index % width, index / width };
	}
};

struct elf
{
	grove_map& map;
	int location;
	int proposal = -1;

	void make_proposal(int round)
	{
		for (int i = 0; i < map.movements.size(); i++)
		{
			int movement_index = (round + i) % map.movements.size();
			auto& movement = map.movements[movement_index];
			if (map.is_free(location, movement))
			{
				proposal = map.propose(location, movement);
				return;
			}
		}
		proposal = -1;
	}

	void make_movement()
	{
		if (proposal >= 0)
		{
			auto& dst = map.data[proposal];
			if (dst == '1')
			{
				dst = '#';
				map.data[location] = '.';
				location = proposal;
			}
			else
			{
				dst = '.';
			}
			proposal = -1;
		}
	}

	bool need_move()
	{
		for (auto& step : map.all_direction_steps)
		{
			if (map.data[location + step] == '#')
			{
				return true;
			}
		}
		return false;
	}
};

std::vector<elf> get_elves(grove_map& map)
{
	std::vector<elf> elves;
	size_t index = map.data.find_first_of('#');
	while (index != std::string::npos)
	{
		elves.emplace_back(map, index);
		index = map.data.find_first_of('#', index + 1);
	}
	return elves;
}

bool perform_round(std::vector<elf>& elves, int round)
{
	bool need_move = false;
	for (auto& elf : elves)
	{
		if (elf.need_move())
		{
			need_move = true;
			elf.make_proposal(round);
		}
	}

	for (auto& elf : elves)
	{
		elf.make_movement();
	}
	return need_move;
}

puzzle<23, 1> X = [](input& input) -> output
{
	constexpr int round_count = 10;

	grove_map map(input.lines, round_count + 1);
	std::vector<elf> elves = get_elves(map);
	for (int i = 0; i < round_count; i++)
	{
		perform_round(elves, i);
	}

	auto min = vector2{ map.width, map.height };
	auto max = vector2{ 0, 0 };	
	for (auto& elf : elves)
	{
		auto pos = map.to_position(elf.location);
		min = vector2::min(min, pos);
		max = vector2::max(max, pos);
	}

	auto delta = max - min;
	auto width = delta.x + 1;
	auto height = delta.y + 1;
	return width * height - elves.size();
};

puzzle<23, 2> X = [](input& input) -> output
{
	auto max_side = std::max(input.lines.front().size(), input.lines.size());
	grove_map map(input.lines, max_side / 2 + 1); // should be enough to stop + sentinel
	std::vector<elf> elves = get_elves(map);

	int round = 0;
	while (perform_round(elves, round++)) {}
	return round;
};