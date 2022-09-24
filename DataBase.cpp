#include "DataBase.hpp"
#include "Battle.hpp"
#include <iostream>

std::vector<State> DataBase::state =
{
	{0,"��",-1},
	{1,"����",-1},
	{10,"����",-1},
};

std::vector<Skill> DataBase::skill =
{
	// 0
	{"�A�^�b�N","[�G1*1]�_���[�W*1.0",2,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -1.0f);
		return 1;
	}},
	// 1
	{"�q�[��","[��1*1]��*2.0",3,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction == t.faction; }))
			i.lock()->giveDamage(u, 2.0f);
		return 1;
	}},
	// 2
	{"�ǌ��F��","[�ǌ�]<0>��2",2,[](Unit& u, Battle& b) {
		for (auto& i : b.target)
			i.lock()->giveState(0, &DataBase::state[0], 2);
		return 1;
	}},
	// 3
	{"�u���C�u","[��]<0>����5",1,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.base == t.base; }))
			i.lock()->giveState(0, &DataBase::state[2], 5);
		return 1;
	}},
	// 4
	{"�f�b�h�p���b�g","[�G2*1]�_���[�W*2.0+<0>��2",6,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; },2))
		{
			auto t = i.lock();
			t->giveDamage(u, -2.0f);
			t->giveState(0, &DataBase::state[0], 2);
		}
		return 1;
	}},
	// 5
	{"�ԑ�","[�G1*1]�_���[�W*8.0",5,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -8.0f);
		return 1;
	}},
	// 6
	{"���G��L","[�G1*1]�_���[�W*4.0",2,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -4.0f);
		return 1;
	}},
	// 7
	{"�A���`�J���[","[�S3*1]<0123>����-8",2,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return true; },3))
		{
			auto t = i.lock();
			for (int i = 0; i < 4; ++i)
			{
				t->giveState(i, nullptr, -8);
			}
		}
		return 1;
	}},
	// 8
	{"�^�[�Q�b�g�F�u���C�Y","[�G1*8]",8,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; },1,10));
		return 1;
	}},
	// 9
	{"�ǌ��FExcecution","[�ǌ�]�_���[�W*8.0",3,[](Unit& u, Battle& b) {
		for (auto& i : b.target)
			i.lock()->giveDamage(u, -8.0f);
		return 1;
	}},
};

