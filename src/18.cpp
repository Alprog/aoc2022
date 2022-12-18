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

using to_vec_func = std::function<vector3(int, int, int)>;
auto to_vectorA(int x, int y, int z) {return vector3{ x, y, z }; };
auto to_vectorB(int x, int y, int z) {return vector3{ z, x, y }; };
auto to_vectorC(int x, int y, int z) {return vector3{ y, z, x }; };

puzzle<18, 1> X = [](input& input) -> output
{
	int size = 20;
	int slice = size * size;
	
	std::vector<bool> map;
	map.resize(size * size * size, false);
	
	auto to_index = [&](vector3 pos)
	{
		return pos.x * slice + pos.y * size + pos.z;
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

	auto search = [&](to_vec_func to_vector)
	{
		bool prev = false;
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int z = 0; z < size; z++)
				{
					auto pos = to_vector(x, y, z);
					auto cur = map[to_index(pos)];
					if (prev != cur)
					{
						total += 1;
					}

					prev = cur;
				}
			}
		}
	};

	search(to_vectorA);
	search(to_vectorB);
	search(to_vectorC);

	return total;
};

puzzle<18, 2> X = [](input& input) -> output
{
	int size = 20;
	int slice = size * size;

	std::vector<char> map;
	map.resize(size * size * size, '.');

	auto to_index = [&](vector3 pos)
	{
		return pos.x * slice + pos.y * size + pos.z;
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
		if (np.x >= 0 && np.x < size && np.y >= 0 && np.y < size && np.z >= 0 && np.z < size)
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
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int z = 0; z < size; z++)
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
	auto search = [&](to_vec_func to_vector)
	{
		char prev = false;
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				for (int z = 0; z < size; z++)
				{
					auto pos = to_vector(x, y, z);
					auto cur = map[to_index(pos)];
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
	};

	search(to_vectorA);
	search(to_vectorB);
	search(to_vectorC);

	return total;
};