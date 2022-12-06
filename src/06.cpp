#include "puzzle_handlers.h"

puzzle<6> X = [](input& input)
{
	auto line = input.lines[0];
	int N = input.is_part_one() ? 4 : 14; // unique_area_length

	constexpr int alphabet_count = 26;
	int arr[alphabet_count] = { 0 };

	int duplicates = 0;
	for (int i = 0; i < line.size(); i++)
	{
		auto index = line[i] - 'a';
		if (++arr[index] > 1)
		{
			duplicates++;
		}
		if (i >= N - 1)
		{
			if (i >= N)
			{
				auto index = line[i - N] - 'a';
				if (--arr[index] > 0)
				{
					duplicates--;
				}
			}
			if (duplicates == 0)
			{
				std::cout << i + 1 << "\n";
				return;
			}
		}
	}

	std::cout << "not found" << "\n";
};