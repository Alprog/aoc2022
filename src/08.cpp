#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>

#if defined(_MSC_VER)
#include <intrin.h>
uint32_t get_leading_zeros(uint32_t value) { return __lzcnt(value); }
uint32_t get_trailing_zeros(uint32_t value) {
	unsigned long result;
	return _BitScanForward(&result, value) ? result : sizeof(uint32_t) * CHAR_BIT;
}
#else
uint32_t get_leading_zeros(uint32_t value) { return __builtin_clz(value); }
uint32_t get_trailing_zeros(uint32_t value) { return __builtin_ctz(value); }
#endif

int look(std::string& height_map, std::vector<bool>& visible_map, int start, int line_offset, int line_count, int step_offset, int step_count)
{
	int total = 0;

	for (int i = 0; i < line_count; i++)
	{
		int max_height = 0;

		int line_start = start + i * line_offset;
		for (int j = 0; j < step_count; j++)
		{
			int index = line_start + j * step_offset;
			int height = height_map[index];
			if (height > max_height)
			{
				max_height = height;
				if (!visible_map[index])
				{
					visible_map[index] = true;
					total++;
				}
			}
		}
	}

	return total;
}

puzzle<8, 1> X = [](input& input) -> output
{
	int total_visible = false;

	auto& lines = input.lines;
	int width = input.lines[0].size();
	int height = input.lines.size();

	auto height_map = str_utils::join(lines, "");
	std::vector<bool> visible_map;
	visible_map.resize(height_map.size(), false);
	
	int total = 0;
	total += look(height_map, visible_map, 0, width, height, 1, width); // right
	total += look(height_map, visible_map, width-1, width, height, -1, width); // left
	total += look(height_map, visible_map, 0, 1, width, width, height); // down
	total += look(height_map, visible_map, (height - 1) * width, 1, width, -width, height); // up

	return total;
};

//=================================================================================================================

void traverse(std::string& height_map, std::vector<int>& visible_map, int start, int line_offset, int line_count, int step_offset, int step_count)
{
	constexpr int max_height = 9;

	for (int line_i = 0; line_i < line_count; line_i++)
	{
		int active_bitmask = 0;
		int nearest_indices[max_height + 1] = { 0 };

		int line_start = start + line_i * line_offset;
		for (int i = 0; i < step_count; i++)
		{
			int index = line_start + i * step_offset;
			int current_height = height_map[index] - '0';

			auto search_mask = active_bitmask & (0xFF << current_height);
			int first_blocking_height = std::min((int)get_trailing_zeros(search_mask), max_height);
			int visible_i = nearest_indices[first_blocking_height];
			int visible_count = i - visible_i;
			visible_map[index] *= visible_count;

			active_bitmask = (active_bitmask >> current_height | 1) << current_height;
			nearest_indices[current_height] = i;
		}
	}
}

puzzle<8, 2> X = [](input& input) -> output
{
	int total_visible = false;

	auto& lines = input.lines;
	int width = input.lines[0].size();
	int height = input.lines.size();

	auto height_map = str_utils::join(lines, "");
	std::vector<int> visible_map;
	visible_map.resize(height_map.size(), 1);

	int total = 0;
	traverse(height_map, visible_map, 0, width, height, 1, width); // right
	traverse(height_map, visible_map, width - 1, width, height, -1, width); // left
	traverse(height_map, visible_map, 0, 1, width, width, height); // down
	traverse(height_map, visible_map, (height - 1) * width, 1, width, -width, height); // up

	auto best_value = 0;
	for (auto& value : visible_map)
	{
		best_value = std::max(value, best_value);
	}

	return best_value;
};