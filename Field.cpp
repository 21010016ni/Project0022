#include "Field.hpp"
#include <random>

extern std::mt19937 engine;

int Field::speed = 60;

void Field::update()
{
	// 仮（現状戦闘終了処理を作っていないので、一定時間戦ったらそれで終了するように）
	static int battleCount = 0;

	// 進行
	if (!pause && ++count >= speed)
	{
		count = 0;
		
		if (battle)
		{
			battle->update();
			if (++battleCount >= 10)
				battle.reset();
		}
		else
		{
			// 戦闘中でなかった場合、適当に会話をさせつつたまに戦闘を発生させる
			
			// 深度を進める
			++alpha;

			// 直前の会話を取得
			auto& prev = Log::latest(Log::talk);

			// 戦闘開始のやつ
			if (!std::uniform_int_distribution{ 0,19 }(engine))
			{
				// キャラクターを取得
				auto wu = get(GetMode::ignore, 0xffffffff);
				// キャラクターが存在していたら先頭開始時の台詞を探し、存在したらそれを発言
				if (!wu.expired())
				{
					auto word = wu.lock()->base->GetWord(Charactor::battle_start, prev);
					if (word)
						Log::push(word);
				}
				battle.reset(new Battle);
				for (auto& i : unit)
				{
					battle->set(i);
				}

				// 仮
				battleCount = 0;
			}
			else
			{
				// 会話
				bool talked = false;
				size_t cpos;

				// もし直前の会話に自分以外の色が含まれていたら、話しかけとして処理する
				if (prev && (cpos = prev.text().find_first_of('#', 1)) != std::u8string::npos)
				{
					// 対象を取得する
					auto wu = get(GetMode::now_in, color(prev.text().substr(cpos, 7)));
					// 対象が存在していたら進む、そうでないなら終了
					if (!wu.expired())
					{
						// 対象の台詞を取得
						auto word = wu.lock()->base->GetWord(Charactor::reaction + color(prev.text().substr(0, 7)), prev);
						// もし対応する台詞があれば、それを発言して会話済みフラグを立てる
						if (word)
						{
							Log::push(word);
							talked = true;
						}
						// 対応する台詞が存在しなければ万能リアクションを取得、それも無いなら発言者を別のキャラクターに変える（通常発言）
						else
						{
							word = wu.lock()->base->GetWord(Charactor::reaction_any, prev);
							if (word)
							{
								Log::push(word);
								talked = true;
							}
						}
					}
				}
				if (!talked)
				{
					// キャラクターを取得
					auto unit = get(GetMode::ignore, 0xffffffff);
					// キャラクターが存在していたら探索時の台詞を探し、存在したらそれを発言
					if (!unit.expired())
					{
						auto word = unit.lock()->base->GetWord(Charactor::search, prev);
						if (word)
							Log::push(word);
					}
				}
			}
		}
	}
}

std::weak_ptr<Unit> Field::get(GetMode mode, int color)
{
	static std::vector<std::weak_ptr<Unit>> v;
	if (v.size() != unit.size())
		for (auto& i : unit)
			v.emplace_back(i);
	std::shuffle(v.begin(), v.end(), engine);
	for (auto& i : v)
	{
		switch (mode)
		{
		case Field::GetMode::ignore:
			return i;
		case Field::GetMode::base_ex:
			if ((i.lock()->base->status.color & 0x00ffffff) != color)
				return i;
			break;
		case Field::GetMode::base_in:
			if ((i.lock()->base->status.color & 0x00ffffff) == color)
				return i;
			break;
		case Field::GetMode::now_ex:
			if ((i.lock()->value.color & 0x00ffffff) != color)
				return i;
			break;
		case Field::GetMode::now_in:
			if ((i.lock()->value.color & 0x00ffffff) == color)
				return i;
			break;
		}
	}
	return std::weak_ptr<Unit>();
}

