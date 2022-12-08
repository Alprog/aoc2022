#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>

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

int calc_visible(std::vector<std::string>& lines, int x, int y, int width, int height)
{
	int a = 0, b = 0, c = 0, d = 0;

	auto tree_height = lines[y][x];
	for (int i = x - 1; i >= 0; i--)
	{
		a++;
		if (lines[y][i] >= tree_height)
		{
			break;
		}
	}

	for (int i = x + 1; i < width; i++)
	{
		b++;
		if (lines[y][i] >= tree_height)
		{
			break;
		}
	}

	for (int i = y - 1; i >= 0; i--)
	{
		c++;
		if (lines[i][x] >= tree_height)
		{
			break;
		}
	}

	for (int i = y + 1; i < height; i++)
	{
		d++;
		if (lines[i][x] >= tree_height)
		{
			break;
		}
	}

	return a * b * c * d;
}

puzzle<8, 2> X = [](input& input) -> output
{
	int total_visible = false;

	auto& lines = input.lines;
	int width = input.lines[0].size();
	int height = input.lines.size();

	int best_visible = 0;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			best_visible = std::max(best_visible, calc_visible(lines, x, y, width, height));
		}
	}

	return best_visible;
};