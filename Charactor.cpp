#include "Charactor.hpp"
#include <random>

extern std::mt19937 engine;

Unit::StateSlot* Unit::hasState(int id)
{
	for (int i = 0; i < 4; ++i)
		if (state[i] == id)
			return &state[i];
	return nullptr;
}

void Unit::giveDamage(Unit& t, float m)
{
	// ��b�_���[�W�v�Z
	float v = t.value.atk * m;
	// ��������
	v *= std::uniform_real_distribution<float>{t.value.tec, t.value.tec + t.value.luc}(engine);
	// ��Ԉُ�̏���
	if (t.hasState(10) != nullptr)
		v *= 1.3f;

	// �h��͏���
	v -= value.def;
	if (m > 0)
		v = __max(v, 0);

	// ���f
	value.hp += (int)v;
	// �\��
	Log::push(Log::Tag::battle, std::format("{} HP {}  ({:+})", base->name, value.hp, (int)v).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// ���f
	if (v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// �\��
	Log::push(Log::Tag::battle, std::format("{}<>{}> {}{}", base->name, slot, (state[slot]) ? state[slot].state->Name() : "", state[slot].time).c_str());
}
