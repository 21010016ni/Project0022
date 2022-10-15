#include "Battle.hpp"
#include <random>
#include <algorithm>
#include <format>
#include "Log.hpp"
#include "Canvas.hpp"
#include "DataBase.hpp"
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
	// ターン数（仮）
	static int turn = 0;

	Log::push(Log::Tag::battle, (ext::convert(std::format("{}", ++turn)) + u8"ターン目").c_str());
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
				Log::push(Log::Tag::battle, (ext::convert(std::format("{}", ext::convert(buf->base->name))) + u8"は毒に侵されて1のダメージ").c_str());
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
			Log::push(Log::Tag::battle, (ext::convert(std::format("{}", ext::convert(buf->base->name))) + u8"の行動").c_str());

			// エフェクトリスト
			std::vector<int> effects;
			// スキル発動数（なんかわけわからんスキル作ったりして変なことになった時も強制的に255回スキル発動したら終了）
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
				Log::push(Log::Tag::battle, (u8"《" + ext::convert(std::format("{}", ext::convert(DataBase::skill[buf->base->skill[num].first].Name()))) + u8"》！").c_str());
				buf->cool += DataBase::skill[buf->base->skill[num].first](*buf, *this, j);
				// エフェクト登録
				if (buf->base->skill[num].second != -1)
					effects.emplace_back(buf->base->skill[num].second);

				// スキル後発言
				word = buf->base->GetWord(Charactor::skill_after + num, pw);
				if(word)
					Log::push(word);
			}
			// エフェクト再生（使用したスキルの中からランダム）
			if (effects.size() > 0)
			{
				auto it = effects.begin();
				std::advance(it, std::uniform_int_distribution<size_t>{0, effects.size() - 1}(random));
				Canvas::PlayEffect(*it);
			}

			// 行動終了文字出力
			Log::push(Log::Tag::battle, (ext::convert(std::format("{}", (int)n)) + u8"回行動した　" + ext::convert(std::format("CT{}", buf->cool))).c_str());
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

