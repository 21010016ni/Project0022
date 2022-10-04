#pragma once
#include <functional>
#include <string>

class Unit;
class Battle;

class Skill
{
	std::string name;
	std::string lore;
	int cool;
	std::function<unsigned int(Unit&, Battle&)> function;

public:
	Skill(std::string name, std::string lore, int cool, std::function<unsigned int(Unit&, Battle&)> function) :name(name), lore(lore), cool(cool), function(function)
	{

	}
	int operator()(Unit& u, Battle& b, unsigned int& p)const
	{
		p += function(u, b);
		return cool;
	}
	const std::string& Name()const
	{
		return name;
	}
};

