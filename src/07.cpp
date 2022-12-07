#include "puzzle_handlers.h"

#include "string_utils.h"
#include <functional>
#include <map>

struct file
{
	std::string name;
	long size;
};

struct dir
{
	dir(dir* parent = nullptr)
		: parent(parent)
	{
	}

	dir* parent = nullptr;
	std::map<std::string, file> files;
	std::map<std::string, dir> dirs;
	long cached_size = 0;

	long calculate_size()
	{
		cached_size = 0;
		for (auto& pair : files)
		{
			cached_size += pair.second.size;
		}
		for (auto& pair : dirs)
		{
			cached_size += pair.second.calculate_size();
		}
		return cached_size;
	}

	void foreach_dir(const std::function<void(dir&)> callback)
	{
		callback(*this);
		for (auto& pair : dirs)
		{
			pair.second.foreach_dir(callback);
		}
	}
};

dir build_fs(input& input)
{
	dir root;
	dir* current_dir = &root;
	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, " ");
		if (arr[0] == "$" && arr[1] == "cd")
		{
			auto& name = arr[2];
			if (name == "/")
			{
				current_dir = &root;
			}
			else if (name == "..")
			{
				current_dir = current_dir->parent;
			}
			else
			{
				current_dir->dirs[name] = dir{ current_dir };
				current_dir = &current_dir->dirs[name];
			}
		}
		else if (arr[0] == "dir")
		{
			// ...
		}
		else // should be file
		{
			long size = std::atol(arr[0].c_str());
			auto& name = arr[1];
			current_dir->files[name] = { name, size };
		}
	}
	return root;
}

puzzle<7, 1> X = [](input& input) -> output
{
	dir root = build_fs(input);
	root.calculate_size();

	long total = 0;
	root.foreach_dir([&](dir& dir)
		{
			if (dir.cached_size <= 100000)
			{
				total += dir.cached_size;
			}
		});

	return total;
};

puzzle<7, 2> X = [](input& input) -> output
{
	dir root = build_fs(input);

	constexpr long full_size = 70000000;
	constexpr long required_size = 30000000;

	long used_size = root.calculate_size();
	long unused_size = full_size - used_size;

	long needed_size = required_size - unused_size;

	long best_size = LONG_MAX;
	root.foreach_dir([&](dir& dir)
	{
		if (dir.cached_size >= needed_size && dir.cached_size < best_size)
		{
			best_size = dir.cached_size;
		}
	});

	return best_size;
};