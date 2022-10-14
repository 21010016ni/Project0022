#include "DataBase.hpp"
#include "Battle.hpp"

std::vector<State> DataBase::state =
{
	{0,u8"��",-1},
	{1,u8"����",-1},
	{10,u8"����",-1},
};

std::vector<Skill> DataBase::skill =
{
	// 0
	{u8"�A�^�b�N",u8"[�G1*1]�_���[�W*1.0",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -1.0f, 0);
		return 1;
	}},
	// 1
	{u8"�q�[��",u8"[��1*1]��*2.0",30,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction == t.faction; }))
			i.lock()->giveDamage(u, 2.0f, 0);
		return 1;
	}},
	// 2
	{u8"�ǌ��F��",u8"[�ǌ�]<0>��2",20,[](Unit& u, Battle& b) {
		for (auto& i : b.target)
			i.lock()->giveState(0, &DataBase::state[0], 2);
		return 1;
	}},
	// 3
	{u8"�u���C�u",u8"[��]<0>����5",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.base == t.base; }))
			i.lock()->giveState(0, &DataBase::state[2], 5);
		return 1;
	}},
	// 4
	{u8"�f�b�h�p���b�g",u8"[�G2*1]���@�_���[�W*2.0+<0>��2",60,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; },2))
		{
			auto t = i.lock();
			t->giveDamage(u, -2.0f, 2);
			t->giveState(0, &DataBase::state[0], 2);
		}
		return 1;
	}},
	// 5
	{u8"�ԑ�",u8"[�G1*1]�����_���[�W*8.0",50,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -8.0f, 1);
		return 1;
	}},
	// 6
	{u8"���G��L",u8"[�G1*1]�����_���[�W*4.0",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; }))
			i.lock()->giveDamage(u, -4.0f, 1);
		return 1;
	}},
	// 7
	{u8"�A���`�J���[",u8"[�S3*1]<0123>����-8",20,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return true; },3))
		{
			auto t = i.lock();
			for (int i = 0; i < 4; ++i)
				t->giveState(i, nullptr, -8);
		}
		return 1;
	}},
	// 8
	{u8"�^�[�Q�b�g�F�u���C�Y",u8"[�G1*8]",80,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction != t.faction; },1,10));
		return 1;
	}},
	// 9
	{u8"�ǌ��FExcecution",u8"[�ǌ�]�����_���[�W*8.0",30,[](Unit& u, Battle& b) {
		for (auto& i : b.target)
			i.lock()->giveDamage(u, -8.0f, 3);
		return 1;
	}},
	// 10
	{u8"�t�B�W�J���q�[��",u8"[��1*1]�����_���[�W*2.0&������*6.0",40,[](Unit& u, Battle& b) {
		for (auto& i : b.get(u, [](const Unit& u, const Unit& t) {return u.faction == t.faction; }))
		{
			auto t = i.lock();
			t->giveDamage(u, -2.0f, 1);
			t->giveDamage(u, 6.0f, 1);
		}
		return 1;
	}},
};

