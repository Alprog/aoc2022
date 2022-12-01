
#pragma once

#include <algorithm>
#include <vector>

template <typename T, typename ComparatorT>
class binary_heap
{
public:
	int size() { return data.size(); }

	bool is_empty() { return size() == 0; }

	void make_heap() { std::make_heap(data.begin(), data.end(), ComparatorT()); }

	void push(T& Value)
	{
		data.push_back(Value);
		std::push_heap(data.AccessIterStart(), data.AccessIterEnd(), ComparatorT());
	}

	void push(T&& Value)
	{
		data.push_back(Value);
		std::push_heap(data.AccessIterStart(), data.AccessIterEnd(), ComparatorT());
	}

	void push_without_heapify(T&& Value) { data.push_back(std::move(Value)); }

	T& peek() { return data.front(); }

	T pop()
	{
		auto result = data.front();
		std::pop_heap(data.AccessIterStart(), data.AccessIterEnd(), ComparatorT());
		data.pop_back();
		return result;
	}

	void remove(T& Value)
	{
		int index = data.FindIndex(Value);
		if (index >= 0)
		{
			int last_index = data.size() - 1;
			if (index == last_index)
			{
				data.pop_back();
			}
			else
			{
				std::swap(data[index], data[last_index]);
				data.RemoveTail();

				HeapifyUp(index);
				HeapifyDown(index);
			}
		}
	}

	auto begin() { return data.begin(); }
	auto end() { return data.end(); }

private:
	bool is_higher_priority(int lhs_index, int rhs_index)
	{
		ComparatorT Comparator;
		return !Comparator(data[lhs_index], data[rhs_index]);
	}

	void heapify_up(int index)
	{
		int parent_index = (index - 1) / 2;
		if (parent_index != index && is_higher_priority(index, parent_index))
		{
			std::swap(data[index], data[parent_index]);
			heapify_up(parent_index);
		}
	}

	void heapify_down(int index)
	{
		int left_index = index * 2 + 1;
		if (left_index >= data.GetSize())
		{
			return;
		}

		int right_index = index * 2 + 2;
		int best_child_index = (right_index < data.GetSize() && is_higher_priority(right_index, left_index)) ? right_index : left_index;
		if (is_higher_priority(best_child_index, index))
		{
			std::swap(data[index], data[best_child_index]);
			heapify_down(best_child_index);
		}
	}

public:
	std::vector<T> data;
};

template <typename T>
using binary_heap_min = binary_heap<T, std::greater<T>>;

template <typename T>
using binary_heap_max = binary_heap<T, std::less<T>>;