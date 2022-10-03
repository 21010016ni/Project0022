#include "Charactor.hpp"
#include <random>

extern std::mt19937 engine;

Unit::StateSlot* Unit::hasState(int id)
{
	for(int i = 0; i < 4; ++i)
		if(state[i] == id)
			return &state[i];
	return nullptr;
}

void Unit::giveDamage(Unit& t, float m, char type)
{
	// ��b�_���[�W�v�Z
	float v = 0.0f;
	switch(type)
	{
	case 0:	// �C�Ӂi�����E���@�̂��������ق�*�{���j
		v = __max(t.value.atk, t.value.mag) * m;
		break;
	case 1:	// �����i�����U����*�{���j
		v = t.value.atk * m;
		break;
	case 2:	// ���p�i���@�U����*�{���j
		v = t.value.mag * m;
		break;
	case 3:	// �����i�����U����*���@�U����*�{��/2�A�h��͂�������x�����j
		v = (t.value.atk + t.value.mag) * m / 2;
		break;
	case 4:	// �����i�󂯂鑤�̌���HP�̊����j
		v = value.hp * m;
		break;
	}
	// ��������
	float r = std::uniform_real_distribution<float>{t.value.tec, t.value.tec + t.value.luc}(engine);
	v *= r;
	// ��Ԉُ�̏���
	if(t.hasState(10) != nullptr)
		v *= 1.3f;

	// �h��͏���
	switch(type)
	{
	case 2:
		v -= value.def / ((t.value.atk + t.value.mag) / 4);
		break;
	default:
		v -= value.def;
	}
	if(m > 0)
		v = __max(v, 0);

	// ���f
	value.hp += (int)v;
	// �\��
	Log::push(Log::Tag::battle, std::format("{} HP {}  (<c,{}>{:+}<c>)", base->name, value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// ���f
	if(v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// �\��
	Log::push(Log::Tag::battle, std::format("{}<>{}> {}{}", base->name, slot, (state[slot]) ? state[slot].state->Name() : "", state[slot].time).c_str());
}
