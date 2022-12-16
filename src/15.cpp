#include "puzzle_handlers.h"

#include "string_utils.h"
#include "vector2.h"
#include <functional>
#include <algorithm>
#include <set>

struct signal
{
	vector2 position;
	int free_distance;
};

struct range
{
	int start;
	int end;

	int size()
	{
		return end - start + 1;
	}

	auto operator<=>(const range& rhs) const
	{
		return end <=> rhs.end;
	}

	static range merge(range& a, range& b)
	{
		return { std::min(a.start, b.start), std::max(a.end, b.end) };
	}
};

puzzle<15, 1> X = [](input& input) -> output
{
	std::vector<signal> signals;
	std::set<vector2> beacons;

	vector2 min{ INT_MAX, INT_MAX };
	vector2 max{ INT_MIN, INT_MIN };

	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, "=");
		auto signal_position = vector2{ std::atoi(arr[1].c_str()), std::atoi(arr[2].c_str()) };
		auto beacon_position = vector2{ std::atoi(arr[3].c_str()), std::atoi(arr[4].c_str()) };
		auto distance = (beacon_position - signal_position).get_manhattan_size();
		signals.emplace_back(signal_position, distance);
		beacons.insert(beacon_position);
	}

	//constexpr int target_row = 10;
	constexpr int target_row = 2000000;

	std::vector<range> ranges;
	for (auto& signal : signals)
	{
		int vert_distance = std::abs(signal.position.y - target_row);
		int offset = signal.free_distance - vert_distance;
		if (offset >= 0)
		{
			ranges.emplace_back(signal.position.x - offset, signal.position.x + offset);
		}
	}

	std::sort(ranges.begin(), ranges.end());

	for (int i = ranges.size() - 1; i > 0; i--)
	{
		auto& a = ranges[i - 1];
		auto& b = ranges[i];
		if (a.end >= b.start)
		{
			a = range::merge(a, b);
			ranges.erase(ranges.begin() + i);
		}
	}

	int total = 0;
	for (auto& range : ranges)
	{
		total += range.size();
	}
	for (auto& beacon : beacons)
	{
		if (beacon.y == target_row)
		{
			total--;
		}
	}

	return total;
};


puzzle<15, 2> X = [](input& input) -> output
{
	std::vector<signal> signals;
	std::set<vector2> beacons;

	vector2 min{ INT_MAX, INT_MAX };
	vector2 max{ INT_MIN, INT_MIN };

	for (auto& line : input.lines)
	{
		auto arr = str_utils::split(line, "=");
		auto signal_position = vector2{ std::atoi(arr[1].c_str()), std::atoi(arr[2].c_str()) };
		auto beacon_position = vector2{ std::atoi(arr[3].c_str()), std::atoi(arr[4].c_str()) };
		auto distance = (beacon_position - signal_position).get_manhattan_size();
		signals.emplace_back(signal_position, distance);
		beacons.insert(beacon_position);
	}

	constexpr int max_line = 4000000;

	vector2 searched_position;
	
	std::vector<range> ranges;
	ranges.reserve(signals.size());

	for (int row = max_line; row >= 0; row--)
	{	
		ranges.clear();

		for (auto& signal : signals)
		{
			int vert_distance = std::abs(signal.position.y - row);
			int offset = signal.free_distance - vert_distance;
			if (offset >= 0)
			{
				ranges.emplace_back(signal.position.x - offset, signal.position.x + offset);
			}
		}

		std::sort(ranges.begin(), ranges.end());

		for (int i = ranges.size() - 1; i > 0; i--)
		{
			auto& a = ranges[i - 1];
			auto& b = ranges[i];
			if (a.end >= b.start - 1)
			{
				a = range::merge(a, b);
				ranges[i] = ranges.back();
				ranges.pop_back();
			}
		}

		if (ranges.size() == 2)
		{
			auto pos = vector2{ ranges[0].end + 1, row };
			if (pos.x >= 0 && pos.x <= max_line)
			{
				searched_position = pos;
				break;
			}
		}
	}

	constexpr int freq_tuning = 4000000;

	long long result = (long long)searched_position.x * (long long)freq_tuning + (long long)searched_position.y;
	return result;
};