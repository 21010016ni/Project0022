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
	// Šî‘bƒ_ƒ[ƒWŒvŽZ
	float v = 0.0f;
	switch(type)
	{
	case 0:	// ”CˆÓi•¨—E–‚–@‚Ì‚¤‚¿‚‚¢‚Ù‚¤*”{—¦j
		v = __max(t.value.atk, t.value.mag) * m;
		break;
	case 1:	// •¨—i•¨—UŒ‚—Í*”{—¦j
		v = t.value.atk * m;
		break;
	case 2:	// –‚pi–‚–@UŒ‚—Í*”{—¦j
		v = t.value.mag * m;
		break;
	case 3:	// •¨–‚i•¨—UŒ‚—Í*–‚–@UŒ‚—Í*”{—¦/2A–hŒä—Í‚ð‚ ‚é’ö“x–³Ž‹j
		v = (t.value.atk + t.value.mag) * m / 2;
		break;
	case 4:	// Š„‡iŽó‚¯‚é‘¤‚ÌŒ»ÝHP‚ÌŠ„‡j
		v = value.hp * m;
		break;
	}
	// —”ˆ—
	float r = std::uniform_real_distribution<float>{t.value.tec, t.value.tec + t.value.luc}(engine);
	v *= r;
	// ó‘ÔˆÙí‚Ìˆ—
	if(t.hasState(10) != nullptr)
		v *= 1.3f;

	// –hŒä—Íˆ—
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

	// ”½‰f
	value.hp += (int)v;
	// •\Ž¦
	Log::push(Log::Tag::battle, std::format("{} HP {}  (<c,{}>{:+}<c>)", base->name, value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// ”½‰f
	if(v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// •\Ž¦
	Log::push(Log::Tag::battle, std::format("{}<>{}> {}{}", base->name, slot, (state[slot]) ? state[slot].state->Name() : "", state[slot].time).c_str());
}
