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

int get_max_line_size(std::vector<std::string>& lines)
{
	size_t max = 0;
	for (auto& line : lines)
	{
		max = std::max(max, line.size());
	}
	return max;
}

int detect_resolution(std::vector<std::string>& lines)
{
	// there are only two possible aspects for cube net: 4:3 and 5:2

	int side_a = get_max_line_size(lines);
	int side_b = lines.size();
	int min_size = std::min(side_a, side_b);
	int max_size = std::max(side_a, side_b);
	if (min_size * 4 == max_size * 3)
	{
		return max_size / 4;
	}
	else if (min_size * 5 == max_size * 2)
	{
		return max_size / 5;
	}
	return -1;
}

struct wormhole
{
	int destination;
	int rotation;
};

struct monkey_map
{
	int width;
	int height;
	std::string data;
	std::vector<int> direction_steps;
	std::map<int, wormhole> wormholes;

	monkey_map(std::vector<std::string>& lines, bool sentinel)
	{
		width = get_max_line_size(lines);
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

		direction_steps = { 1, width, -1, -width };
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

struct cursor
{
	cursor(monkey_map& map)
		: map { map }
	{
		index = map.data.find_first_of('.');
		step = map.direction_steps[direction];
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
			index += map.direction_steps[direction];
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
		step = map.direction_steps[direction];
	}

	void draw_self()
	{
		map.data[index] = direction_arrows[direction];
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
		: target(reinterpret_cast<face&>(*this)) // garbage, will be overriden
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

	int index = 0;
	std::vector<link> links;

	int minimap_index = 0;
	vector2 net_position;
	int net_rotation = 0;
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

	static void connect_faces(face& face_a, face& face_b, int forward, int backward)
	{
		face_a.links[forward] = link(face_b, wrap(backward + 2));
		face_b.links[backward] = link(face_a, wrap(forward + 2));
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

void find_net_locations(cube& cube, monkey_map& minimap)
{
	int index = minimap.data.find_first_of('#');
	
	face& start_face = cube.faces[0];
	start_face.minimap_index = index;
	minimap.data[index] = start_face.index + '0';

	using face_vector = std::vector<std::reference_wrapper<face>>;
	face_vector front{ start_face };
	while (!front.empty())
	{
		face_vector new_faces;

		for (face& cur_face : front)
		{
			for (int net_direction = 0; net_direction < direction_count; net_direction++)
			{
				auto step = minimap.direction_steps[net_direction];
				int new_index = cur_face.minimap_index + step;
				if (minimap.data[new_index] == '#')
				{
					auto face_direction = wrap(net_direction - cur_face.net_rotation);
					auto& link = cur_face.links[face_direction];
					face& new_face = link.target;
					new_face.minimap_index = new_index;
					minimap.data[new_index] = new_face.index + '0';

					auto backward_direction = new_face.get_direction(cur_face);
					auto net_backward_direction = wrap(net_direction + 2);

					new_face.net_rotation = wrap(net_backward_direction - backward_direction);
					
					new_faces.push_back(new_face);
				}
			}
		}

		front.clear();
		std::swap(front, new_faces);
	}

	for (auto& face : cube.faces)
	{
		face.net_position = minimap.to_position(face.minimap_index); 
	}
}

void convert_net_position(cube& cube, int resolution)
{
	// convert positions from minimap to large mapn
	for (auto& face : cube.faces)
	{
		face.net_position -= vector2(1, 1); // remove sentinel
		face.net_position *= resolution;
		face.net_position += vector2(1, 1); // add sentinel
	}
}

void setup_wormholes(cube& cube, monkey_map& map, int resolution)
{
	for (auto& face : cube.faces)
	{
		auto start_pos = face.net_position;
		map.data[map.to_index(start_pos)] = '$';
		
	}
}

void solve_cube_wormholes(monkey_map& map, std::vector<std::string>& lines)
{
	cube cube;

	int resolution = detect_resolution(lines);
	auto minimap_input_lines = filter_minimap_lines(lines, resolution);
	monkey_map minimap(minimap_input_lines, true);
	find_net_locations(cube, minimap);
	convert_net_position(cube, resolution);
	setup_wormholes(cube, map, resolution);

	//for (auto& face : cube.faces)
	//{
	//	std::cout << "face" << face.index << " rotated " << -face.net_rotation * 90 << " degree\n";
	//}
	//std::cout << std::endl << minimap.data;
}

puzzle<22> X = [](input& input) -> output
{
	if (!input.is_test)
		return 0;

	std::string sequence = input.lines.back();
	input.lines.resize(input.lines.size() - 2);

	monkey_map map(input.lines, true);
	cursor cursor(map);
	
	if (input.is_part_two())
	{
		solve_cube_wormholes(map, input.lines);

		std::cout << map.data;
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