#include "puzzle_handlers.h"

#include "string_utils.h"
#include "vector2.h"
#include <functional>
#include <bitset>

struct figure
{
	int64_t height = 0;

	figure(uint16_t a, uint16_t b = 0, uint16_t c = 0, uint16_t d = 0)
	{
		lines[0] = a;
		lines[1] = b;
		lines[2] = c;
		lines[3] = d;
		for (auto& line : lines)
		{
			if (line != 0)
			{
				height++;
			}
		}
	}

	union
	{
		uint64_t pattern;
		uint16_t lines[4];
	};

	auto print()
	{
		for (int i = 3; i >= 0; i--)
		{
			std::cout << std::bitset<9>(lines[i]) << std::endl;
		}
	}
};

std::vector<figure> figures = {
	figure(0b1111'0000),
	figure(0b0100'0000,
		   0b1110'0000, 
		   0b0100'0000), 
	figure(0b1110'0000,
		   0b0010'0000,
		   0b0010'0000),
	figure(0b1000'0000,
		   0b1000'0000, 
		   0b1000'0000,
		   0b1000'0000),
	figure(0b1100'0000,
		   0b1100'0000) 
};

puzzle<17> X = [](input& input) -> output
{
	constexpr int line_width = 7;
	constexpr int spawn_offset_x = 2;
	constexpr int spawn_offset_y = 3;

	auto shifts = input.lines[0];
	auto wall_pattern = (1 << 0) | (1 << (line_width + 1));
	figure wall(wall_pattern, wall_pattern, wall_pattern, wall_pattern);

	std::vector<uint16_t> stack;
	std::vector<uint16_t> walls;
	stack.resize(10000, wall_pattern);
	walls.resize(10000, wall_pattern);
	stack[0] = -1; // floor

	int figure_index = -1;
	int shift_index = -1;
	int overflow = -1;

	auto get_next_figure = [&]() 
	{
		figure_index = (figure_index + 1) % figures.size();
		return figures[figure_index];
	};

	auto apply_next_shift = [&](uint64_t figure)
	{
		shift_index = (shift_index + 1) % shifts.size();
		return shifts[shift_index] == '<' ? figure << 1 : figure >> 1;
	};

	int64_t accumulated = 0;
	int64_t tower_height = 0;

	uint64_t rock_count = input.is_part_one() ? 2022 : 1'000'000'000'000;

	uint64_t last_height = 0;
	uint64_t last_rocks = 0;
	bool first_visit = true;

	for (uint64_t i = 0; i < rock_count; i++)
	{
		if (shift_index == 1) // 0 for test
		{
			if (first_visit)
			{
				last_height = accumulated + tower_height;
				last_rocks = i;
				first_visit = false;
			}
			else
			{
				uint64_t rock_per_iteration = i - last_rocks;
				uint64_t height_per_iteration = tower_height - last_height;
				uint64_t iteration = (rock_count - i) / rock_per_iteration;
				i += rock_per_iteration * iteration;
				accumulated += height_per_iteration * iteration;
			}
		}

		auto figure = get_next_figure();
		auto figure_pattern = figure.pattern >> spawn_offset_x;

		uint16_t* stack_ptr = &stack[tower_height + spawn_offset_y + 1];
		while (true)
		{
			uint64_t& stack_segment = *reinterpret_cast<uint64_t*>(stack_ptr);

			auto shifted = apply_next_shift(figure_pattern);
			if ((shifted & stack_segment) == 0)
			{
				figure_pattern = shifted;
			}

			uint64_t& next_segment = *reinterpret_cast<uint64_t*>(--stack_ptr);
			if ((next_segment & figure_pattern) != 0)
			{
				int cur_line_height = stack_ptr - &stack[0];
				tower_height = std::max(tower_height, cur_line_height + figure.height);
				stack_segment = stack_segment | figure_pattern;
				break;
			}
		}
	}

	return accumulated + tower_height;
};