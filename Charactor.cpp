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

void Unit::giveDamage(Unit& t, float m, char type)
{
	// 基礎ダメージ計算
	float v = 0.0f;
	switch (type)
	{
	case 0:	// 任意（物理・魔法のうち高いほう*倍率）
		v = __max(t.value.atk, t.value.mag) * m;
		break;
	case 1:	// 物理（物理攻撃力*倍率）
		v = t.value.atk * m;
		break;
	case 2:	// 魔術（魔法攻撃力*倍率）
		v = t.value.mag * m;
		break;
	case 3:	// 物魔（物理攻撃力*魔法攻撃力*倍率/2、防御力をある程度無視）
		v = (t.value.atk + t.value.mag) * m / 2;
		break;
	case 4:	// 割合（受ける側の現在HPの割合）
		v = value.hp * m;
		break;
	}
	// 乱数処理
	float r = std::uniform_real_distribution<float>{ t.value.tec, t.value.tec + t.value.luc }(engine);
	v *= r;
	// 状態異常の処理
	if (t.hasState(10) != nullptr)
		v *= 1.3f;

	// 防御力処理
	switch (type)
	{
	case 2:
		v -= value.def / ((t.value.atk + t.value.mag) / 4);
		break;
	default:
		v -= value.def;
	}
	if (m > 0)
		v = __max(v, 0);

	// 反映
	value.hp += (int)v;
	// 表示
	Log::push(Log::Tag::battle, std::format("{} HP {}  (<c,{}>{:+}<c>)", base->name, value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v).c_str());
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
