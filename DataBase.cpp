#include "DataBase.hpp"
#include "Battle.hpp"

std::vector<State> DataBase::state =
{
	{0,"毒",-1},
	{1,"炎上",-1},
	{10,"興奮",-1},
};

std::vector<Skill> DataBase::skill =
{
	// 0
	{"アタック","[敵1*1]ダメージ*1.0",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -1.0f, 0);
		return 1;
	}},
	// 1
	{"ヒール","[味1*1]回復*2.0",30,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction == t.faction; }))
			i.lock()->giveDamage(u, 2.0f, 0);
		return 1;
	}},
	// 2
	{"追撃：毒","[追撃]<0>毒2",20,[](Unit& u, Battle& b) {
		for (auto& i : b.target)
			i.lock()->giveState(0, &DataBase::state[0], 2);
		return 1;
	}},
	// 3
	{"ブレイブ","[自]<0>興奮5",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.base == t.base; }))
			i.lock()->giveState(0, &DataBase::state[2], 5);
		return 1;
	}},
	// 4
	{"デッドパレット","[敵2*1]魔法ダメージ*2.0+<0>毒2",60,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; },2))
		{
			auto t = i.lock();
			t->giveDamage(u, -2.0f, 2);
			t->giveState(0, &DataBase::state[0], 2);
		}
		return 1;
	}},
	// 5
	{"花葬","[敵1*1]物理ダメージ*8.0",50,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -8.0f, 1);
		return 1;
	}},
	// 6
	{"膚エ裂キ","[敵1*1]物理ダメージ*4.0",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -4.0f, 1);
		return 1;
	}},
	// 7
	{"アンチカラー","[全3*1]<0123>効果-8",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return true; },3))
		{
			auto t = i.lock();
			for (int i = 0; i < 4; ++i)
				t->giveState(i, nullptr, -8);
		}
		return 1;
	}},
	// 8
	{"ターゲット：ブレイズ","[敵1*8]",80,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; },1,10));
		return 1;
	}},
	// 9
	{"追撃：Excecution","[追撃]物魔ダメージ*8.0",30,[](Unit& u, Battle& b) {
		for (auto& i : b.target)
			i.lock()->giveDamage(u, -8.0f, 3);
		return 1;
	}},
	// 10
	{"フィジカルヒール","[味1*1]物理ダメージ*2.0&物理回復*6.0",40,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction == t.faction; }))
		{
			auto t = i.lock();
			t->giveDamage(u, -2.0f, 1);
			t->giveDamage(u, 6.0f, 1);
		}
		return 1;
	}},
};

