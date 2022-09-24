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
	// 基礎ダメージ計算
	float v = t.value.atk * m;
	// 乱数処理
	v *= std::uniform_real_distribution<float>{t.value.tec, t.value.tec + t.value.luc}(engine);
	// 状態異常の処理
	if (t.hasState(10) != nullptr)
		v *= 1.3f;

	// 防御力処理
	v -= value.def;
	if (m > 0)
		v = __max(v, 0);

	// 反映
	value.hp += (int)v;
	// 表示
	Log::push(Log::Tag::battle, std::format("{} HP {}  ({:+})", base->name, value.hp, (int)v).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// 反映
	if (v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// 表示
	Log::push(Log::Tag::battle, std::format("{}<>{}> {}{}", base->name, slot, (state[slot]) ? state[slot].state->Name() : "", state[slot].time).c_str());
}
