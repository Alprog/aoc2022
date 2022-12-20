#pragma once

template <typename T>
struct auto_cleanup_vector : public std::vector<T*>
{
	using base = std::vector<T*>;

	~auto_cleanup_vector()
	{
		for (int i = 0; i < base::size(); i++)
		{
			delete base::at(i);
		}
	}
};