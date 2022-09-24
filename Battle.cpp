#include "Battle.hpp"
#include <random>
#include <algorithm>
#include <format>
#include "Log.hpp"

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
	//std::cout << ++turn << "ターン目" << std::endl;
	Log::push(Log::Tag::battle, std::format("{}ターン目", ++turn).c_str());
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
				Log::push(Log::Tag::battle, std::format("{}は毒に侵されて1のダメージ", buf->base->name).c_str());
				buf->value.hp -= 1;
				break;
			}

			// 効果時間の減少
			--s;
		}

		// ステータス表示
		//std::cout << buf->base->name << std::endl << "HP : " << buf->value.hp << std::endl;
		Log::push(Log::Tag::battle, std::format("{} HP : {}", buf->base->name, buf->value.hp).c_str());

		// スキル発動
		if(--buf->cool <= 0)
		{
			//std::cout << buf->base->name << "の行動" << std::endl;
			Log::push(Log::Tag::battle, std::format("{}の行動", buf->base->name).c_str());
			unsigned char n = 0;
			for(size_t j = 0; j < buf->base->skill.size() && n < 255; ++n)
			{
				//std::cout << "<" << buf->base->skill[j].Name() << ">！" << std::endl;
				Log::push(Log::Tag::battle, std::format("《{}》！", buf->base->skill[j].Name()).c_str());
				buf->cool += buf->base->skill[j](*buf, *this, j);
			}
			//std::cout << (int)n << "回行動した　CT" << buf->cool << std::endl << std::endl;
			Log::push(Log::Tag::battle, std::format("{}回行動した　CT", (int)n, buf->cool).c_str());
		}
	}
	//std::cout << "ターン終了" << std::endl << std::endl;
	Log::push(Log::Tag::battle, "ターン終了");
	Log::push(Log::Tag::battle, "");
}

std::vector<std::weak_ptr<Unit>>& Battle::get(const Unit& u, const std::function<bool(const Unit&, const Unit&)>& regulation, unsigned short num, unsigned short count)
{
	target.clear();
	while(count-- > 0)
	{
		auto& buf = ShuffledUnit();
		unsigned short i = 0;
		for(auto it = buf.cbegin(); it != buf.cend() && i < num; ++it)
		{
			if(regulation(u, *(it->lock())))
			{
				target.emplace_back(*it);
				++i;
			}
		}
	}
	return target;
}

