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
	// Šî‘bƒ_ƒ[ƒWŒvŽZ
	float v = t.value.atk * m;
	// —”ˆ—
	float r = std::uniform_real_distribution<float>{ t.value.tec, t.value.tec + t.value.luc }(engine);
	v *= r;
	// ó‘ÔˆÙí‚Ìˆ—
	if (t.hasState(10) != nullptr)
		v *= 1.3f;

	// –hŒä—Íˆ—
	v -= value.def;
	if (m > 0)
		v = __max(v, 0);

	// ”½‰f
	value.hp += (int)v;
	// •\Ž¦
	Log::push(Log::Tag::battle, std::format("{} HP {}  (<c,{}>{:+}<c>)", base->name, value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// ”½‰f
	if (v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// •\Ž¦
	Log::push(Log::Tag::battle, std::format("{}<>{}> {}{}", base->name, slot, (state[slot]) ? state[slot].state->Name() : "", state[slot].time).c_str());
}
