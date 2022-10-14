#include "Battle.hpp"
#include <random>
#include <algorithm>
#include <format>
#include "Log.hpp"
#include "Canvas.hpp"
#include "convert_string.hpp"

std::mt19937 random(std::random_device{}());

std::vector<std::weak_ptr<Unit>>& Battle::ShuffledUnit()
{
	static std::vector<std::weak_ptr<Unit>> ret;
	if(ret.size() != unit.size())
	{
		ret.clear();
		for(const auto& i : unit)
			ret.emplace_back(i);
	}
	std::shuffle(ret.begin(), ret.end(), random);
	return ret;
}

void Battle::update()
{
	static int turn = 0;
	Log::push(Log::Tag::battle, ext::convert(std::format("{}ターン目", ++turn)).c_str());
	target.clear();
	auto move = ShuffledUnit();
	for(auto& i : move)
	{
		auto buf = i.lock();

		// 状態異常の処理
		for(auto& s : buf->state)
		{
			// 行動時に処理が入る状態異常
			switch(s.id())
			{
			case 0:	// 毒
				Log::push(Log::Tag::battle, ext::convert(std::format("{}は毒に侵されて1のダメージ", ext::convert(buf->base->name))).c_str());
				buf->value.hp -= 1;
				break;
			}

			// 効果時間の減少
			--s;
		}

		// ステータス表示
		Log::push(Log::Tag::battle, ext::convert(std::format("{} HP : {}", ext::convert(buf->base->name), buf->value.hp)).c_str());

		// スキル発動
		if((buf->cool -= buf->value.spd) <= 0)
		{
			buf->cool = 0;
			Log::push(Log::Tag::battle, ext::convert(std::format("{}の行動", ext::convert(buf->base->name))).c_str());
			unsigned char n = 0;
			for(unsigned int j = 0; j < buf->base->skill.size() && n < 255; ++n)
			{
				// スキル前発言
				auto num = j;
				auto& pw = Log::latest(Log::talk);
				auto word = buf->base->GetWord(Charactor::skill_prev + num, pw);
				if(word)
					Log::push(word);

				// スキル発動
				Log::push(Log::Tag::battle, ext::convert(std::format("《{}》！", ext::convert(buf->base->skill[j].Name()))).c_str());
				buf->cool += buf->base->skill[j](*buf, *this, j);

				// スキル後発言
				word = buf->base->GetWord(Charactor::skill_after + num, pw);
				if(word)
					Log::push(word);
			}
			Canvas::PlayEffect(0);
			Log::push(Log::Tag::battle, ext::convert(std::format("{}回行動した　CT", (int)n, buf->cool)).c_str());
		}
	}
	Log::push(Log::Tag::battle, u8"ターン終了");
	Log::push(Log::Tag::battle, u8"");
}

std::vector<std::weak_ptr<Unit>>& Battle::get(const Unit& u, const std::function<bool(const Unit&, const Unit&)>& regulation, unsigned short num, unsigned short count, bool overload)
{
	std::vector<std::weak_ptr<Unit>>* table;
	if(overload)
	{
		table = &target;
	}
	else
	{
		static std::vector<std::weak_ptr<Unit>> over;
		table = &over;
	}

	table->clear();
	while(count-- > 0)
	{
		auto& buf = ShuffledUnit();
		unsigned short i = 0;
		for(auto it = buf.cbegin(); it != buf.cend() && i < num; ++it)
		{
			if(regulation(u, *(it->lock())))
			{
				table->emplace_back(*it);
				++i;
			}
		}
	}
	return *table;
}

