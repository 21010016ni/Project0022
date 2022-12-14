#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "Status.hpp"
#include "Skill.hpp"
#include "State.hpp"
#include "Log.hpp"

#pragma warning(disable:26812)

class Charactor
{
public:
	enum Situation :unsigned int
	{
		search,
		dead,
		battle_start,
		skill_prev = 0xb0000000,
		skill_after = 0xb1000000,
		reaction_any = 0xfe000000,
		reaction = 0xff000000,
	};

	std::u8string name;
	Status status;
	std::vector<std::pair<int, int>> skill;	// スキル番号,エフェクト番号
	std::multimap<unsigned int, std::u8string> word;
	struct Graph
	{
		std::u8string prof;
		std::u8string icon;
	} graph;

	Charactor(const char8_t* name, int color, int hp, int atk, int def, int mag, int reg, int spd, float tec, float luc)
		:name(name), status(color, hp, atk, def, mag, reg, spd, tec, luc)
	{
	}
	Charactor(const std::u8string& name, const Status& status) :name(name), status(status)
	{
	}

	const Log::Text GetWord(unsigned int key, const Log::Text& prev, bool speaker = true)const;

	static Charactor load(const char* FileName);
	void out(const std::string& Directory);
};

class Unit
{
	struct StateSlot
	{
		int time;
		State* state;

		StateSlot() :time(0), state(nullptr) {}
		int id() { return (*this) ? *state : -1; }
		operator bool() { return state != nullptr; }
		int& operator--() { return *this -= 1; }
		int& operator+=(int v) { if((time = __max(time + v, 0)) == 0)state = nullptr; return time; }
		int& operator-=(int v) { if((time = __max(time - v, 0)) == 0)state = nullptr; return time; }
		bool operator==(int id) { return (*this) ? *state == id : false; }
	};

public:
	Charactor* base;

	enum class Faction
	{
		player,
		enemy
	}faction;

	Status value;
	int cool;
	StateSlot state[4];

	Unit(Charactor& charactor, Faction faction) :base(&charactor), faction(faction), value(charactor.status), cool(0) {}

	StateSlot* hasState(int id);
	void giveDamage(Unit& t, float m, char type);
	void giveState(int slot, State* v, int time);
};

