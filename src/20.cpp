#include "puzzle_handlers.h"

#include "string_utils.h"
#include "auto_cleanup_vector.h"
#include <functional>

struct node
{
	int64_t value;
	node* next = nullptr;
	node* prev = nullptr;

	node* get(int64_t offset)
	{
		auto cur = this;
		while (offset > 0)
		{
			cur = cur->next;
			offset--;
		}
		while (offset < 0)
		{
			cur = cur->prev;
			offset++;
		}
		return cur;
	}

	void shift(int64_t count)
	{
		if (count > 0)
		{
			auto before = this->get(count);
			insert(before, before->next);
		}
		else if (count < 0)
		{
			auto after = this->get(count);
			insert(after->prev, after);
		}
	}

	void insert(node* before, node* after)
	{
		this->prev->next = this->next;
		this->next->prev = this->prev;
		this->prev = before;
		this->next = after;
		before->next = this;
		after->prev = this;
	}
};

puzzle<20> X = [](input& input) -> output
{
	auto_cleanup_vector<node> nodes;

	node* start_node = nullptr;
	for (auto& line : input.lines)
	{
		auto value = std::atoi(line.c_str());
		nodes.push_back(new node{ value });
		if (value == 0)
		{
			start_node = nodes.back();
		}
	}

	if (input.is_part_two())
	{
		int64_t key = 811589153;
		for (auto& node : nodes)
		{
			node->value *= key;
		}
	}

	for (int i = 0; i < nodes.size() - 1; i++)
	{
		nodes[i]->next = nodes[i + 1];
		nodes[i + 1]->prev = nodes[i];
	}
	nodes.front()->prev = nodes.back();
	nodes.back()->next = nodes.front();

	int wrap = nodes.size() - 1;
	int count = input.is_part_one() ? 1 : 10;
	for (int i = 0; i < count; i++)
	{
		for (auto& node : nodes)
		{
			node->shift(node->value % wrap);
		}
	}

	auto a = start_node->get(1000 % nodes.size());
	auto b = start_node->get(2000 % nodes.size());
	auto c = start_node->get(3000 % nodes.size());
	return a->value + b->value + c->value;
};