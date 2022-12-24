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

int invert(int direction)
{
	return wrap(direction + 2);
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
	int destinationA;
	int directionA;
	int destinationB;
	int directionB;
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

	void add_wormhole(int place, int destination, int direction)
	{
		auto it = wormholes.find(place);
		if (it == wormholes.end())
		{
			wormholes[place] = { destination, direction };
		}
		else
		{
			// alternative path
			it->second.destinationB = destination;
			it->second.directionB = direction;
		}
	}
};

struct cursor
{
	cursor(monkey_map& map)
		: map { map }
	{
		index = map.data.find_first_of('.');
	}

	void move(int count)
	{
		for (int i = 0; i < count; i++)
		{
			int step = map.direction_steps[direction];
			int next_index = index + step;
			int next_direction = direction;
			if (map.data[next_index] == ' ')
			{
				next_index = pacman_teleport(next_index, step);
			}
			else if (map.data[next_index] == '@')
			{
				auto& wormhole = map.wormholes[next_index];
				if (wormhole.destinationA != index)
				{
					next_index = wormhole.destinationA;
					next_direction = wormhole.directionA;
				}
				else
				{
					next_index = wormhole.destinationB;
					next_direction = wormhole.directionB;
				}
			}

			if (map.data[next_index] == '#')
			{
				break;
			}

			draw_self();
			index = next_index;
			direction = next_direction;
		}

		draw_self();
	}

	int pacman_teleport(int index, int step)
	{
		do
		{
			index -= step;
		} while (map.data[index] != ' ');
		return index + step;
	}

	void rotate_direction(int& direction, int rotation)
	{
		direction = wrap(direction + rotation);
	}

	void rotate(int rotation)
	{
		rotate_direction(direction, rotation);
	}

	void draw_self()
	{
		map.data[index] = direction_arrows[direction];
	}

	int index;
	int direction = 0;
	monkey_map& map;
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
	std::vector<int> edge_anchors;
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
		face_a.links[forward] = link(face_b, invert(backward));
		face_b.links[backward] = link(face_a, invert(forward));
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
					auto net_backward_direction = invert(net_direction);

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

void calculate_edge_anchors(cube& cube, monkey_map& map, int resolution)
{
	for (auto& face : cube.faces)
	{
		// convert positions from minimap to large map
		auto map_position = face.net_position;
		map_position -= vector2(1, 1); // remove sentinel
		map_position *= resolution;
		map_position += vector2(1, 1); // add sentinel
	
		auto add_anchor = [&](int x, int y) 
		{
			auto anchor_position = map_position + vector2{ x, y };
			face.edge_anchors.push_back(map.to_index(anchor_position));
		};

		add_anchor(resolution, 0);
		add_anchor(resolution - 1, resolution);
		add_anchor(-1, resolution - 1);
		add_anchor(0, -1);
	}
}

void setup_wormholes(cube& cube, monkey_map& map, int resolution)
{
	for (auto& src_face : cube.faces)
	{	
		for (int src_net_edge = 0; src_net_edge < direction_count; src_net_edge++)
		{
			auto& src_anchor = src_face.edge_anchors[src_net_edge];
			if (map.data[src_anchor] == ' ' || map.data[src_anchor] == '@')
			{
				auto src_local_direction = wrap(src_net_edge - src_face.net_rotation);
				auto& link = src_face.links[src_local_direction];
				face& dst_face = link.target;
				
				auto dst_net_edge = wrap(dst_face.net_rotation + invert(link.direction));
				
				auto src_step = map.direction_steps[wrap(src_net_edge + 1)];
				auto dst_step = map.direction_steps[wrap(dst_net_edge + 1)];

				auto dst_anchor = dst_face.edge_anchors[dst_net_edge];
				dst_anchor += (resolution - 1) * dst_step;

				auto dst_net_direction = invert(dst_net_edge);
				dst_anchor += map.direction_steps[dst_net_direction];

				for (int i = 0; i < resolution; i++)
				{
					auto src_index = src_anchor + i * src_step;
					auto dst_index = dst_anchor - i * dst_step;
					map.data[src_index] = '@';
					map.add_wormhole(src_index, dst_index, dst_net_direction);
				}
			}
		}
	}
}

void solve_cube_wormholes(monkey_map& map, std::vector<std::string>& lines)
{
	cube cube;

	int resolution = detect_resolution(lines);
	auto minimap_input_lines = filter_minimap_lines(lines, resolution);
	monkey_map minimap(minimap_input_lines, true);
	find_net_locations(cube, minimap);
	calculate_edge_anchors(cube, map, resolution);
	setup_wormholes(cube, map, resolution);

	//uncomment to print minimap
	//std::cout << std::endl << minimap.data;
}

puzzle<22> X = [](input& input) -> output
{
	std::string sequence = input.lines.back();
	input.lines.resize(input.lines.size() - 2);

	monkey_map map(input.lines, true);
	cursor cursor(map);
	
	if (input.is_part_two())
	{
		solve_cube_wormholes(map, input.lines);
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

	//uncomment to print path
	//std::cout << map.data;

	auto pos = map.to_position(cursor.index);
	return pos.y * 1000 + pos.x * 4 + cursor.direction;
};