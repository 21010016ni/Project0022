#pragma once
#include <compare>
#include <functional>
#include <string>

class Unit;

class State
{
	int id;
	std::u8string name;
	int icon;

public:
	State(int id, std::u8string name, int icon) :name(name), id(id), icon(icon)
	{

	};
	const char8_t* Name()const { return name.c_str(); }
	int Icon()const { return icon; }
	auto operator<=>(const State& t) { return id <=> t.id; }
	auto operator<=>(int id) { return id <=> id; }
	operator int()const { return id; }
};

