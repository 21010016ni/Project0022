#include "Manager.hpp"
#include <DxLib.h>
#include "Icon.hpp"
#include "Log.hpp"
#include "DataBase.hpp"
#include "BGM.hpp"
#include "Input.hpp"
#include "Effect.hpp"

void Manager::CountReset()
{
	count = 0;
}

void Manager::preset()
{
	font = LoadFontDataToHandle("data/font/TTEdit2_3ゴシック.dft");
	Icon::load("data/picture/icon.png", 16);
	Log::SetFont(font);
	Menu::SetFont(font);
	BGM::set("data/bgm/Changeling.mp3");
	BGM::set("data/bgm/Etude.mp3");
	BGM::set("data/bgm/なんだか明るい感じのワルツ.wav");
	BGM::set("data/bgm/夢.mp3");
	BGM::set("data/bgm/宝石と想いを胸に.mp3");
	BGM::set("data/bgm/自然.mp3");

	// Charactor(name,color,hp,atk,def,mag,reg,spd,tec,luc)
	charactor.emplace_back("テスト1", 0xffffffff, 30, 5, 0, 0, 0, 10, 0.8f, 0.5f);
	charactor.back().skill.push_back(DataBase::skill[4]);
	charactor.emplace_back("テスト2", 0xffffffff, 300, 8, 0, 0, 0, 10, 0.8f, 0.5f);
	charactor.back().skill.push_back(DataBase::skill[3]);
	charactor.back().skill.push_back(DataBase::skill[1]);
	charactor.emplace_back("テスト3", 0xffffffff, 800, 10, 0, 0, 0, 10, 0.0f, 1.4f);
	charactor.back().skill.push_back(DataBase::skill[8]);
	charactor.back().skill.push_back(DataBase::skill[9]);

	battle.set(charactor[0], Unit::Faction::player);
	battle.set(charactor[1], Unit::Faction::player);
	battle.set(charactor[2], Unit::Faction::enemy);

	Menu::root.emplace_back(0x5e5, "操作");
	Menu::root.back().emplace_back(0x621, "パレット云々");
	Menu::root.back().emplace_back(0x621, "戦闘スピード");
	Menu::root.back().emplace_back(0x338, "戦闘進行", [](int, Menu::Item& i) { Manager::pause ^= true; Manager::CountReset(); if(Manager::pause) { i.icon = 0x350; } else { i.icon = 0x338; } });
	Menu::root.emplace_back(0x5f7, "実績");
	Menu::root.back().emplace_back(0x624, "いろいろ");
	Menu::root.emplace_back(0x5f4, "設定");
	Menu::root.back().emplace_back(0x628, "ログ表示");
	Menu::root.back().back().emplace_back(0x320, "戦闘", [](int, Menu::Item& i) { Log::mute[Log::Tag::battle] ^= true; if(Log::mute[Log::Tag::battle]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, "システム", [](int, Menu::Item& i) { Log::mute[Log::Tag::system] ^= true; if(Log::mute[Log::Tag::system]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, "会話", [](int, Menu::Item& i) { Log::mute[Log::Tag::talk] ^= true; if(Log::mute[Log::Tag::talk]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, "デバッグ", [](int, Menu::Item& i) { Log::mute[Log::Tag::debug] ^= true; if(Log::mute[Log::Tag::debug]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().emplace_back(0x629, "自動セーブ");
	
	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem("data/se/刀剣・斬る01.mp3"));

	volume.bgm = 64;
	volume.se = 64;

	BGM::volume = (volume.Bgm()) ? volume.bgm : 0;
	Effect::volume = (volume.Se()) ? volume.se : 0;
}

void Manager::update()
{
	if(!pause && ++count >= speed)
	{
		count = 0;
		battle.update();
	}
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
		Effect::set(0, 200, 0, Effect::Pos::leftup);
	}
	if(Mouse::wheel() != 0)
	{
		if(Log::hit(Mouse::pos()))
		{
			textline = __max(__min(textline - Mouse::wheel(), Log::maxNum), 0);
		}
	}
	if(BGM::update())
	{
		BGMCount = 600;
	}
}

void Manager::draw()
{
	Log::draw(textline);
	Menu::draw(Mouse::pos());
	Effect::play();
	if(BGMCount > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, BGMCount);
		DrawStringToHandle(1020 - GetDrawStringWidthToHandle(BGM::title.c_str(), (int)BGM::title.size(), font), 4, BGM::title.c_str(), 0xfffffff, font);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		BGMCount -= 2;
	}
}

