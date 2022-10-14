#pragma once
#include <functional>
#include <string>

class Unit;
class Battle;

class Skill
{
	std::u8string name;
	std::u8string lore;
	int cool;
	std::function<unsigned int(Unit&, Battle&)> function;

public:
	Skill(const std::u8string& name, const std::u8string& lore, int cool, std::function<unsigned int(Unit&, Battle&)> function) :name(name), lore(lore), cool(cool), function(function)
	{

	}
	int operator()(Unit& u, Battle& b, unsigned int& p)const
	{
		p += function(u, b);
		return cool;
	}
	const std::u8string& Name()const
	{
		return name;
	}
};

