#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>

struct vector3
{
	int x = 0;
	int y = 0;
	int z = 0;

	vector3 operator+(const vector3& rhs)
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}

	vector3 operator-(const vector3& rhs)
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}
};

puzzle<18, 1> X = [](input& input) -> output
{
	int width = 20;
	int height = 20;
	int depth = 20;
	int slice = height * depth;
	
	std::vector<bool> map;
	map.resize(width * height * depth, false);
	
	auto to_index = [&](vector3 pos)
	{
		return pos.x * slice + pos.y * depth + pos.z;
	};

	std::vector<vector3> cubes;
	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, ",");
		auto x = std::atoi(arr[0].c_str());
		auto y = std::atoi(arr[1].c_str());
		auto z = std::atoi(arr[2].c_str());
		cubes.emplace_back(x, y, z);
		map[to_index({ x, y, z })] = true;
	}

	int total = 0;

	bool prev = false;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				auto cur = map[to_index({ x, y, z })];
				if (prev != cur)
				{
					total += 1;
				}

				prev = cur;
			}
		}
	}

	prev = false;
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			for (int y = 0; y < height; y++)
			{
				auto cur = map[to_index({ x, y, z })];
				if (prev != cur)
				{
					total += 1;
				}

				prev = cur;
			}
		}
	}

	prev = false;
	for (int y = 0; y < height; y++)
	{
		for (int z = 0; z < depth; z++)
		{
			for (int x = 0; x < width; x++)
			{
				auto cur = map[to_index({ x, y, z })];
				if (prev != cur)
				{
					total += 1;
				}

				prev = cur;
			}
		}
	}

	return total;
};

puzzle<18, 2> X = [](input& input) -> output
{
	int width = 20;
	int height = 20;
	int depth = 20;
	int slice = height * depth;

	std::vector<char> map;
	map.resize(width * height * depth, '.');

	auto to_index = [&](vector3 pos)
	{
		return pos.x * slice + pos.y * depth + pos.z;
	};

	std::vector<vector3> cubes;
	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, ",");
		auto x = std::atoi(arr[0].c_str());
		auto y = std::atoi(arr[1].c_str());
		auto z = std::atoi(arr[2].c_str());
		cubes.emplace_back(x, y, z);
		map[to_index({ x, y, z })] = '#';
	}

	map[0] = '0';

	auto check = [&](vector3 pos, vector3 offset)
	{
		auto np = pos + offset;
		if (np.x >= 0 && np.x < width && np.y >= 0 && np.y < height && np.z >= 0 && np.z < depth)
		{
			if (map[to_index(np)] == '0')
			{
				map[to_index(pos)] = '0';
				return true;
			}
		}
		return false;
	};


	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				for (int z = 0; z < depth; z++)
				{
					vector3 pos = { x,y, z };
					auto cur = map[to_index(pos)];
					if (cur == '.')
					{
						changed |= check(pos, { 0,0,1 }) | check(pos, { 0,0,-1 }) | check(pos, { 0,1,0 }) | check(pos, { 0,-1,0 }) | check(pos, { 1,0,0 }) | check(pos, { -1,0,0 });
					}
				}
			}
		}
	}


	int total = 0;

	char prev = false;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				auto cur = map[to_index({ x, y, z })];
				if (prev != cur)
				{
					if ((cur == '#' && prev == '0') || (cur == '0' && prev == '#'))
					{
						total += 1;
					}
				}

				prev = cur;
			}
		}
	}

	prev = false;
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			for (int y = 0; y < height; y++)
			{
				auto cur = map[to_index({ x, y, z })];
				if (prev != cur)
				{
					if ((cur == '#' && prev == '0') || (cur == '0' && prev == '#'))
					{
						total += 1;
					}
				}

				prev = cur;
			}
		}
	}

	prev = false;
	for (int y = 0; y < height; y++)
	{
		for (int z = 0; z < depth; z++)
		{
			for (int x = 0; x < width; x++)
			{
				auto cur = map[to_index({ x, y, z })];
				if (prev != cur)
				{
					if ((cur == '#' && prev == '0') || (cur == '0' && prev == '#'))
					{
						total += 1;
					}
				}

				prev = cur;
			}
		}
	}

	//for (int x = 0; x < width; x++)
	//{
	//	for (int y = 0; y < height; y++)
	//	{
	//		for (int z = 0; z < depth; z++)
	//		{
	//			std::cout << map[to_index({ x, y, z })];
	//		}
	//		std::cout << std::endl;
	//	}
	//	std::cout << std::endl;
	//	std::cout << std::endl;
	//}

	return total;
};