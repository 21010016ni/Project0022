#include "Charactor.hpp"
#include <format>
#include <random>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"
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
	Log::push(Log::Tag::battle, ext::convert(std::format("{} HP {}  (<c,{}>{:+}<c>)", ext::convert(base->name), value.hp, (r > 1.2f) ? 0xffffaa : ((v < 0) ? 0xffaaaa : 0xaaffaa), (int)v)).c_str());
}

void Unit::giveState(int slot, State* v, int time)
{
	// 反映
	if(v != nullptr)
		state[slot].state = v;
	state[slot] += time;
	// 表示
	Log::push(Log::Tag::battle, ext::convert(std::format("{}<>{}> {}{}", ext::convert(base->name), slot, (state[slot]) ? ext::convert(state[slot].state->Name()) : "", state[slot].time)).c_str());
}

const Log::Text Charactor::GetWord(unsigned int key, const Log::Text& prev, bool speaker)const
{
	static std::u8string buf;
	std::vector<std::u8string> elem;

	for(auto i = word.equal_range(key); i.first != i.second; ++i.first)
	{
		// キーワードが指定されているかをチェック
		auto p = i.first->second.find_first_of('|');
		if(p != std::u8string::npos)
		{
			// 指定されていたら、その位置までを切り出してprevに含まれているかを検索、含まれていなかったら次の条件へ
			if (prev && prev.text().find(i.first->second.substr(0, p)) == std::u8string::npos)
				continue;
			buf = i.first->second.substr(p + 1);
		}
		else
		{
			// 指定されていなかった場合、そのまま
			buf = i.first->second;
		}

		// 特殊文字置換
		while((p = buf.find_first_of('[')) != std::u8string::npos)
		{
			auto q = buf.find_first_of(']', p);
			if(q == std::u8string::npos)
				throw;
			boost::split(elem, buf.substr(p + 1, q - p - 1), boost::is_any_of(","));
			auto it = elem.begin();
			std::advance(it, std::uniform_int_distribution<size_t>{0, elem.size() - 1}(engine));
			buf.replace(p, q - p + 1, *it);
		}
		while((p = buf.find_first_of('@')) != std::u8string::npos)
		{
			int code = 0;
			std::weak_ptr<Unit> unit;
			while (true)
			{
				switch (buf.at(p + 1))
				{
				case 'm':	// 自分自身の色
					code = status.color;
					break;
				case 'p':	// 直前の発言者の色

					if (prev && prev.HasSpeaker())
						code = color(prev.text().substr(0, 7));
					else
						goto SelectWordRoopEnd;
					break;
				case 'a':	// フルカラーの中からランダムの色
					do
					{
						code = std::uniform_int_distribution{ 0x000000,0xffffff }(engine);
					} while (code == (status.color & 0x00ffffff));
					break;
				case 'r':	// 現在いるすべてのキャラクターの色
					// まだ
					goto SelectWordRoopEnd;
					break;
				case 'f':	// 現在フィールド上にいるキャラクターの色（変化していた場合、本来の色）
					unit = Field::get(GetMode::ex | GetMode::base, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					code = unit.lock()->base->status.color;
					break;
				case 'n':	// 現在フィールド上にいるキャラクターの色（変化していた場合、変化後の色）
					unit = Field::get(GetMode::ex | GetMode::now, status.color);
					if (unit.expired())
						goto SelectWordRoopEnd;
					code = unit.lock()->value.color;
					break;
				case 'e':	// 戦闘が発生していた場合、その相手の色
					// まだ
					goto SelectWordRoopEnd;
					break;
				}
				break;
			}
			buf.replace(p, 2, color(code));
		}
		if (speaker)
		{
			buf.insert(0, color(status.color));
		}

		return Log::Text(Log::talk, buf);

	SelectWordRoopEnd:
		continue;
	}
	return Log::Text();
}

Charactor Charactor::load(const char* FileName)
{
	std::ifstream ifs(FileName, std::ios::binary);
	if (!ifs.is_open())
		throw std::invalid_argument("faiilure open file");

	// バッファ作成
	unsigned char length = 0;
	unsigned char size = 0;
	std::u8string sbuf;
	Status status;
	int ibuf0 = 0;
	int ibuf1 = 0;

	// データを読み込む

	// 名前(可変長)
	ifs.read(reinterpret_cast<char*>(&length), 1);
	sbuf.resize(length);
	ifs.read(reinterpret_cast<char*>(sbuf.data()), length);

	// ステータス
	ifs.read(reinterpret_cast<char*>(&status.color), 3);
	ifs.read(reinterpret_cast<char*>(&status.hp), 4);
	ifs.read(reinterpret_cast<char*>(&status.atk), 4);
	ifs.read(reinterpret_cast<char*>(&status.def), 4);
	ifs.read(reinterpret_cast<char*>(&status.mag), 4);
	ifs.read(reinterpret_cast<char*>(&status.reg), 4);
	ifs.read(reinterpret_cast<char*>(&status.spd), 4);
	ifs.read(reinterpret_cast<char*>(&status.tec), 4);
	ifs.read(reinterpret_cast<char*>(&status.luc), 4);

	// キャラクター作成
	Charactor charactor(sbuf, status);

	// スキル
	ifs.read(reinterpret_cast<char*>(&size), 1);
	for (unsigned char i = 0; i < size; ++i)
	{
		// スキルID
		ifs.read(reinterpret_cast<char*>(&ibuf0), 4);
		// エフェクトID
		ifs.read(reinterpret_cast<char*>(&ibuf1), 4);
		// スキル登録
		charactor.skill.emplace_back(ibuf0, ibuf1);
	}
	// 台詞
	ifs.read(reinterpret_cast<char*>(&size), 1);
	for (unsigned char i = 0; i < size; ++i)
	{
		// 台詞キー
		ifs.read(reinterpret_cast<char*>(&ibuf0), 4);
		// 台詞内容
		ifs.read(reinterpret_cast<char*>(&length), 1);
		sbuf.resize(length);
		ifs.read(reinterpret_cast<char*>(sbuf.data()), length);
		// スキル登録
		charactor.word.emplace(ibuf0, sbuf);
	}
	return charactor;
}

void Charactor::out(const std::string& Directory)
{
	std::ofstream ofs(Directory + ext::convert(color(status.color).substr(1)) + ".bin", std::ios::binary);
	if (!ofs.is_open())
		throw std::invalid_argument("faiilure create file");
	
	unsigned char size = 0;	// 可変長・個数データの数をメモリに置いておくためのバッファ
	// サイズについては、現状255バイト以上の名前・台詞・スキル個数なんて想定してないのでとりあえず現状これで
	// ダメそうだったら拡張する

	// データを書き込む

	// 名前（可変長）
	ofs.write(reinterpret_cast<const char*>(&(size = static_cast<unsigned char>(name.size()))), 1);
	ofs.write(ext::tochar(name), name.size());

	// ステータス
	ofs.write(reinterpret_cast<const char*>(&status.color), 3);
	ofs.write(reinterpret_cast<const char*>(&status.hp), 4);
	ofs.write(reinterpret_cast<const char*>(&status.atk), 4);
	ofs.write(reinterpret_cast<const char*>(&status.def), 4);
	ofs.write(reinterpret_cast<const char*>(&status.mag), 4);
	ofs.write(reinterpret_cast<const char*>(&status.reg), 4);
	ofs.write(reinterpret_cast<const char*>(&status.spd), 4);
	ofs.write(reinterpret_cast<const char*>(&status.tec), 4);
	ofs.write(reinterpret_cast<const char*>(&status.luc), 4);

	// スキル(可変個数)
	ofs.write(reinterpret_cast<const char*>(&(size = static_cast<unsigned char>(skill.size()))), 1);
	for (const auto& i : skill)
	{
		// スキル番号
		ofs.write(reinterpret_cast<const char*>(&i.first), 4);
		// エフェクト番号
		ofs.write(reinterpret_cast<const char*>(&i.second), 4);
	}

	// 台詞(可変個数)
	ofs.write(reinterpret_cast<const char*>(&(size = static_cast<unsigned char>(word.size()))), 1);
	for (const auto& i : word)
	{
		// キー(4)unsignd int
		ofs.write(reinterpret_cast<const char*>(&i.first), 4);
		// 台詞中身(可変長)
		ofs.write(reinterpret_cast<const char*>(&(size = static_cast<unsigned char>(i.second.size()))), 1);
		ofs.write(ext::tochar(i.second), i.second.size());
	}
}

