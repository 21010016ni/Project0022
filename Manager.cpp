#include "Manager.hpp"
#include <DxLib.h>
#include "Icon.hpp"
#include "Log.hpp"
#include "DataBase.hpp"
#include "BGM.hpp"
#include "Field.hpp"
#include "Input.hpp"
#include "Effect.hpp"
#include "Canvas.hpp"
#include "convert_string.hpp"

void Manager::preset()
{
	font = LoadFontDataToHandle((const char*)u8"data/font/TTEdit2_3ゴシック.dft");
	Icon::load("data/picture/icon.png", 16);
	Log::SetFont(font);
	Menu::SetFont(font);
	Menu::SetSE(LoadSoundMem("data/se/select.mp3"), LoadSoundMem("data/se/decision.mp3"));
	Canvas::back = LoadGraph("data/picture/back00.jpg");
	//BGM::set("data/bgm/Changeling.mp3");
	//BGM::set("data/bgm/Etude.mp3");
	//BGM::set("data/bgm/なんだか明るい感じのワルツ.wav");
	//BGM::set("data/bgm/夢.mp3");
	//BGM::set("data/bgm/宝石と想いを胸に.mp3");
	//BGM::set("data/bgm/自然.mp3");

	// Charactor(name,color,hp,atk,def,mag,reg,spd,tec,luc)
	charactor.emplace_back(u8"アルコン", 0xff0000ff, 300, 20, 10, 20, 0, 10, 0.8f, 0.5f);
	charactor.back().skill.push_back(DataBase::skill[4]);
	charactor.back().word.emplace(Charactor::search, u8"「#ffff00、あー……その、[体調はどうだ,腹減ってないか]」");
	charactor.back().word.emplace(Charactor::reaction + 0xffff00, u8"「へいき|「いや、まあ……そんならいい」");
	charactor.back().word.emplace(Charactor::reaction + 0xffff00, u8"健忘|「違わい！　いや、まあ……ならいいけど」");
	charactor.back().word.emplace(Charactor::reaction + 0xffff00, u8"ステーキ|「こんなとこで無茶な……帰ったらな」");
	charactor.back().word.emplace(Charactor::reaction_any, u8"元気|「まあ……ぼちぼちってとこか。サンキュ」");
	charactor.back().word.emplace(Charactor::reaction_any, u8"？|「[あ、悪い……聞いてなかった,ちょっとわかんねえな……すまん]」");
	charactor.back().word.emplace(Charactor::reaction_any, u8"「おっす」");
	charactor.back().word.emplace(Charactor::battle_start, u8"「[さて、やるか,手早く終わらせるぞ]」");
	charactor.back().word.emplace(Charactor::skill_prev, u8"「[あいよ,ほら、いくぞ,よっこらせ]」");
	charactor.back().word.emplace(Charactor::skill_after, u8"「ま、こんなもん」");
	charactor.emplace_back(u8"レプリカ", 0xffffff00, 300, 8, 0, 0, 0, 10, 0.8f, 0.5f);
	charactor.back().skill.push_back(DataBase::skill[3]);
	charactor.back().skill.push_back(DataBase::skill[1]);
	charactor.back().word.emplace(Charactor::reaction + 0x0000ff, u8"体調はどう|「へいき。ありがと、#0000ff」");
	charactor.back().word.emplace(Charactor::reaction + 0x0000ff, u8"腹減ってない|「[……？　さっきたべた。#0000ff、もう忘れた？　健忘？,おれ、ステーキが食べたい。用意して、#0000ff]」");
	charactor.back().word.emplace(Charactor::reaction_any, u8"？|「[べつに……,しらない]」");
	charactor.back().word.emplace(Charactor::reaction_any, u8"「[……なに。うるさい,はなすことないよ]」");
	charactor.back().word.emplace(Charactor::battle_start, u8"「[……がるる,手、出していい？　……ちぇ]」");
	charactor.emplace_back(u8"なんかつよい敵", 0xffff88ff, 800, 10, 0, 0, 0, 10, 0.2f, 1.4f);
	charactor.back().skill.push_back(DataBase::skill[8]);
	charactor.back().skill.push_back(DataBase::skill[9]);
	charactor.back().word.emplace(Charactor::skill_prev, u8"「さて、始めよう」");
	charactor.back().word.emplace(Charactor::skill_prev + 1, u8"「<c,0xff4444>……処刑<c>」");
	charactor.back().word.emplace(Charactor::battle_start, u8"「[咎人よ、悔い改めよ,開始する]」");

	Field::set(charactor[0], Unit::Faction::player);
	Field::set(charactor[1], Unit::Faction::player);
	Field::set(charactor[2], Unit::Faction::enemy);

	Menu::root.emplace_back(0x338, u8"戦闘進行", [](int, Menu::Item& i) { Field::pause ^= true; Field::CountReset(); if (Field::pause) { i.icon = 0x350; } else { i.icon = 0x338; } });
	Menu::root.emplace_back(0x5e5, u8"操作");
	Menu::root.back().emplace_back(0x621, u8"パレット云々");
	Menu::root.back().emplace_back(0x621, u8"戦闘スピード");
	Menu::root.emplace_back(0x5f7, u8"実績");
	Menu::root.back().emplace_back(0x624, u8"いろいろ");
	Menu::root.emplace_back(0x5f4, u8"設定");
	Menu::root.back().emplace_back(0x628, u8"ログ表示");
	Menu::root.back().back().emplace_back(0x320, u8"戦闘", [](int, Menu::Item& i) { Log::mute[Log::Tag::battle] ^= true; if(Log::mute[Log::Tag::battle]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, u8"システム", [](int, Menu::Item& i) { Log::mute[Log::Tag::system] ^= true; if(Log::mute[Log::Tag::system]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, u8"会話", [](int, Menu::Item& i) { Log::mute[Log::Tag::talk] ^= true; if(Log::mute[Log::Tag::talk]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, u8"デバッグ", [](int, Menu::Item& i) { Log::mute[Log::Tag::debug] ^= true; if(Log::mute[Log::Tag::debug]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().emplace_back(0x629, u8"自動セーブ");

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem("data/se/刀剣・斬る01.mp3"));

	volume.mute &= 0b11111110;

	BGM::volume = volume.Bgm() ? volume.bgm : 0;
	Effect::volume = volume.Se() ? volume.se : 0;
	Menu::SetSEVolume(volume.Se() ? volume.se : 0);
}

void Manager::update()
{
	Field::update();

	// 入力受付
	if(Mouse::click(MOUSE_INPUT_1))
	{
		if(Log::hit(Mouse::pos()))
		{
			textline = 0;
		}
		if(Menu::hit(Mouse::pos()))
		{
			Menu::select(Mouse::pos());
		}
	}
	if(Mouse::wheel() != 0)
	{
		if(Log::hit(Mouse::pos()))
		{
			textline = __max(__min(textline + Mouse::wheel(), Log::maxNum), 0);
		}
	}

	// BGM更新処理
	if(BGM::update())
	{
		BGMCount = 600;
	}
}

void Manager::draw()
{
	Log::draw(textline);
	Menu::draw(Mouse::pos());
	//Canvas::draw();
	Effect::play();
	int num = 4;
	for (int i = 0; i < num; ++i)
	{
		DrawBox(414 + 30 * (i % 2), ((480 - num * 60) / (num + 1)) * (i + 1) + 60 * i, 414 + 60 + 30 * (i % 2), ((480 - num * 60) / (num + 1)) * (i + 1) + 60 * (i + 1), 0xffffffff, TRUE);
	}
	num = 5;
	for (int i = 0; i < num; ++i)
	{
		DrawBox(904 + 30 * (i % 2), ((480 - num * 60) / (num + 1)) * (i + 1) + 60 * i, 904 + 60 + 30 * (i % 2), ((480 - num * 60) / (num + 1)) * (i + 1) + 60 * (i + 1), 0xffffffff, TRUE);
	}
	if(BGMCount > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, BGMCount);
		DrawStringToHandle(1020 - GetDrawStringWidthToHandle(ext::tochar(BGM::title), (int)BGM::title.size(), font), 4, ext::tochar(BGM::title), 0xfffffff, font);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		BGMCount -= 2;
	}
}

