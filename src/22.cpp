#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <map>
#include "vector2.h"

int const right = 0;
int const down = 1;
int const left = 2;
int const up = 3;

int const direction_count = 4;
int const faces_count = 6;

std::vector<char> direction_arrows = { '>', 'V', '<', '^' };

int wrap(int direction)
{
	return (direction + direction_count) % direction_count;
}

struct wormhole
{
	int destination;
	int rotation;
};

struct monkey_map
{
	monkey_map(std::vector<std::string>& lines, bool sentinel)
	{
		width = 0;
		for (auto& line : lines)
		{
			width = std::max(width, (int)line.size());
		}
		height = lines.size();

		if (sentinel)
		{
			width += 2;
			height += 2;
		}

		width++; // for /n

		data.resize(width * height, ' ');

		int index = sentinel;
		for (auto& line : lines)
		{
			memcpy(&data[sentinel] + width * index++, &line[0], line.size());
		}

		for (int i = 0; i < height; i++)
		{
			data[width * (i + 1) - 1] = '\n';
		}

		directions_steps = { 1, width, -1, -width };
	}

	bool is_block(int index)
	{
		if (index >= 0 && index < data.size())
		{
			return data[index] != ' ' && data[index] != '$';
		}
		return false;
	}

	void find_corners()
	{
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] == ' ')
			{
				int total_blocks = 0;
				int pattern = 0;
				for (auto& step : directions_steps)
				{
					total_blocks += is_block(i + step);
				}
				if (total_blocks >= 2)
				{
					data[i] = '$';
				}
			}
		}
	}

	int to_index(vector2 position)
	{
		return position.x + position.y * width;
	}

	vector2 to_position(int index)
	{
		return { index % width, index / width };
	}

	int width;
	int height;
	std::string data;
	std::vector<int> directions_steps;

	std::map<int, wormhole> wormholes;
};

struct cursor
{
	cursor(monkey_map& map)
		: map { map }
	{
		index = map.data.find_first_of('.');
		step = map.directions_steps[direction];
	}

	void move(int count)
	{
		for (int i = 0; i < count; i++)
		{
			int new_index = index + step;
			int new_direction = direction;
			if (map.data[new_index] == ' ')
			{
				teleport(new_index, new_direction);
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

	void teleport(int& index, int& direction)
	{
		if (map.wormholes.size() > 0)
		{
			auto& wormhole = map.wormholes[index];
			rotate_direction(direction, wormhole.rotation);
			index += map.directions_steps[direction];
		}
		else
		{
			do
			{
				index -= step;
			} while (map.data[index] != ' ');
			index += step;
		}
	}

	void rotate_direction(int& direction, int rotation)
	{
		direction = wrap(direction + rotation);
	}

	void rotate(int rotation)
	{
		rotate_direction(direction, rotation);
		step = map.directions_steps[direction];
	}

	void draw_self()
	{
		//map.data[index] = direction_arrows[direction];
	}

	int index;
	int direction = 0;
	monkey_map& map;
	int step;
};

struct face;

struct link
{
	link()
		: target(reinterpret_cast<face&>(*this))
		, direction{ 0 }
	{
	}

	link(std::reference_wrapper<face> target, int direction)
		: target{target}
		, direction{direction}
	{
	}

	std::reference_wrapper<face> target;
	int direction;
};

struct face
{
	face()
	{
		links.resize(direction_arrows.size());
	}

	int get_direction(face& face)
	{
		for (int direction = 0; direction < direction_count; direction++)
		{
			if (&links[direction].target.get() == &face)
			{
				return direction;
			}
		}
		return -1;
	}

	int index;
	std::vector<link> links;

	int unwrap_index = 0;
	int unwrap_rotation = 0;
};

/*                                           
      +-----+
	  |     |
	  |  4  |
      |     |
+-----+-----+-----+-----+
|     |     |     |     |
|  0  |  1  |  2  |  3  |
|     |     |     |     |
+-----+-----+-----+-----+
	  |     |
	  |  5  |
	  |     |
	  +-----+
*/

void connect_faces(face& face_a, face& face_b, int forward, int backward)
{
	face_a.links[forward] = link(face_b, wrap(backward + 2));
	face_b.links[backward] = link(face_a, wrap(forward + 2));
}

struct cube
{
	std::vector<face> faces;

	cube()
	{
		faces.resize(faces_count);

		for (int i = 0; i < faces.size(); i++)
		{
			faces[i].index = i;
		}

		connect_faces(faces[0], faces[1], right, left);
		connect_faces(faces[1], faces[2], right, left);
		connect_faces(faces[2], faces[3], right, left);
		connect_faces(faces[3], faces[0], right, left);

		connect_faces(faces[0], faces[4], up, left);
		connect_faces(faces[1], faces[4], up, down);
		connect_faces(faces[2], faces[4], up, right);
		connect_faces(faces[3], faces[4], up, up);

		connect_faces(faces[0], faces[5], down, left);
		connect_faces(faces[1], faces[5], down, up);
		connect_faces(faces[2], faces[5], down, right);
		connect_faces(faces[3], faces[5], down, down);
	}
};

std::vector<std::string> filter_minimap_lines(std::vector<std::string>& lines, int resolution)
{
	std::vector<std::string> result;
	for (int i = 0; i < lines.size(); i += resolution)
	{
		auto line = lines[i];
		auto& output_line = result.emplace_back();
		for (int j = 0; j < line.size(); j += resolution)
		{
			output_line += line[j] == ' ' ? ' ' : '#';
		}
	}
	return result;
}

void find_unwrap_locations(cube& cube, monkey_map& minimap)
{
	int index = minimap.data.find_first_of('#');
	
	face& start_face = cube.faces[0];
	start_face.unwrap_index = index;
	minimap.data[index] = start_face.index + '0';

	using face_vector = std::vector<std::reference_wrapper<face>>;
	face_vector front{ start_face };
	while (!front.empty())
	{
		face_vector new_faces;

		for (face& cur_face : front)
		{
			for (int unwrap_direction = 0; unwrap_direction < direction_count; unwrap_direction++)
			{
				auto step = minimap.directions_steps[unwrap_direction];
				int new_index = cur_face.unwrap_index + step;
				if (minimap.data[new_index] == '#')
				{
					auto face_direction = wrap(unwrap_direction - cur_face.unwrap_rotation);
					auto& link = cur_face.links[face_direction];
					face& new_face = link.target;
					minimap.data[new_index] = new_face.index + '0';

					auto backward_direction = new_face.get_direction(cur_face);
					auto unwrap_backward_direction = wrap(unwrap_direction + 2);

					new_face.unwrap_rotation = wrap(unwrap_backward_direction - backward_direction);
					new_face.unwrap_index = new_index;
					
					new_faces.push_back(new_face);
				}
			}
		}

		front.clear();
		std::swap(front, new_faces);
	}
}

void solve_cube_wormholes(monkey_map& map, std::vector<std::string>& lines, int resolution)
{
	cube cube;

	auto minimap_input_lines = filter_minimap_lines(lines, resolution);
	monkey_map minimap(minimap_input_lines, true);
	find_unwrap_locations(cube, minimap);

	for (auto& face : cube.faces)
	{
		std::cout << "face" << face.index << " rotated " << -face.unwrap_rotation * 90 << " degree\n";
	}
	std::cout << std::endl << minimap.data;
}

puzzle<22> X = [](input& input) -> output
{
	std::string sequence = input.lines.back();
	input.lines.resize(input.lines.size() - 2);

	monkey_map map(input.lines, true);
	cursor cursor(map);
	
	if (input.is_part_two())
	{
		int resolution = input.is_test ? 4 : 50;
		solve_cube_wormholes(map, input.lines, resolution);

		return 0;
	}

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
	
	cursor.draw_self();

	//std::cout << map.data;

	auto pos = map.to_position(cursor.index);
	return pos.y * 1000 + pos.x * 4 + cursor.direction;
};