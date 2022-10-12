#include "Charactor.hpp"
#include <format>
#include <random>
#include <boost/algorithm/string.hpp>
#include "string_mb_function.hpp"
#include "Field.hpp"

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
	// 基礎ダメージ計算
	float v = 0.0f;
	switch(type)
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
	float r = std::uniform_real_distribution<float>{t.value.tec, t.value.tec + t.value.luc}(engine);
	v *= r;
	// 状態異常の処理
	if(t.hasState(10) != nullptr)
		v *= 1.3f;

	// 防御力処理
	switch(type)
	{
	case 3:
		v += value.def / ((t.value.atk + t.value.mag) / 4.0f);
		break;
	default:
		v += value.def;
	}
	if(m > 0)
		v = __max(v, 0);

	// 反映
	value.hp += (int)v;
	// 表示
	Log::push(Log::Tag::battle, std::format("{} HP {}  (<c,{}>{:+}<c>)", base->name, value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// 反映
	if(v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// 表示
	Log::push(Log::Tag::battle, std::format("{}<>{}> {}{}", base->name, slot, (state[slot]) ? state[slot].state->Name() : "", state[slot].time).c_str());
}

const std::string* Charactor::GetWord(unsigned int key, const std::string& prev, bool speaker)const
{
	static std::string buf;
	std::vector<std::string> elem;

	for(auto i = word.equal_range(key); i.first != i.second; ++i.first)
	{
		// キーワードが指定されているかをチェック
		auto p = ext::find_first_of_mb(i.first->second, '|');
		if(p != std::string::npos)
		{
			// 指定されていたら、その位置までを切り出してprevに含まれているかを検索、含まれていなかったら次の条件へ
			if(prev.find(i.first->second.substr(0, p)) == std::string::npos)
				continue;
			buf = i.first->second.substr(p + 1);
		}
		else
		{
			// 指定されていなかった場合、そのまま
			buf = i.first->second;
		}

		if (speaker)
		{
			buf.insert(0, std::format("#{:06x}", status.color & 0x00ffffff));
		}

		// 特殊文字置換
		while((p = ext::find_first_of_mb(buf, '[')) != std::string::npos)
		{
			auto q = ext::find_first_of_mb(buf, ']', p);
			if(q == std::string::npos)
				throw;
			boost::split(elem, buf.substr(p + 1, q - p - 1), boost::is_any_of(","));
			auto it = elem.begin();
			std::advance(it, std::uniform_int_distribution<size_t>{0, elem.size() - 1}(engine));
			buf.replace(p, q - p + 1, *it);
		}
		while((p = ext::find_first_of_mb(buf,'@')) != std::string::npos)
		{
			int color = 0;
			std::weak_ptr<Unit> unit;
			while (true)
			{
				switch (buf.at(p + 1))
				{
				case 'm':	// 自分自身の色
					color = status.color;
					break;
				case 'p':	// 直前の発言者の色
					if (prev.at(0) == '#')
						color = std::stoi(prev.substr(1, 6), nullptr, 16);
					else
						goto SelectWordRoopEnd;
					break;
				case 'a':	// フルカラーの中からランダムの色
					do
					{
						color = std::uniform_int_distribution{ 0x000000,0xffffff }(engine);
					} while (color == (status.color & 0x00ffffff));
					break;
				case 'r':	// 現在いるすべてのキャラクターの色
					// まだ
					goto SelectWordRoopEnd;
					break;
				case 'f':	// 現在フィールド上にいるキャラクターの色（変化していた場合、本来の色）
					unit = Field::get(Field::GetMode::base_ex, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					color = unit.lock()->base->status.color;
					break;
				case 'n':	// 現在フィールド上にいるキャラクターの色（変化していた場合、変化後の色）
					unit = Field::get(Field::GetMode::now_ex, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					color = unit.lock()->value.color;
					break;
				case 'e':	// 戦闘が発生していた場合、その相手の色
					// まだ
					goto SelectWordRoopEnd;
					break;
				}
				break;
			}
			buf.replace(p, 2, std::format("#{:06x}", color & 0x00ffffff));
		}
		return &buf;
	SelectWordRoopEnd:
		continue;
	}
	return nullptr;
}

