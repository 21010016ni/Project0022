#include "Manager.hpp"
#include <DxLib.h>
#include "Icon.hpp"
#include "Log.hpp"
#include "DataBase.hpp"
#include "Input.hpp"

Menu Manager::menu({ 0,0 }, { 400,384 }, { 24,24 });

void Manager::CountReset()
{
	count = 0;
}

void Manager::preset()
{
	font = LoadFontDataToHandle("data/font/TTEdit2_3ゴシック_24_0.dft");
	Log::SetFont(font);
	menu.SetFont(font);
	Icon::load("data/picture/icon.png", 16);

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

	menu.root.emplace_back(0x5e5, "操作");
	menu.root.back().emplace_back(0x621, "パレット云々");
	menu.root.back().emplace_back(0x621, "戦闘スピード");
	menu.root.back().emplace_back(0x338, "戦闘進行", [](int, Menu::Item& i) {Manager::pause ^= true; Manager::CountReset(); if (Manager::pause) { i.icon = 0x350; } else { i.icon = 0x338; }});
	menu.root.emplace_back(0x5f7, "実績");
	menu.root.back().emplace_back(0x624, "いろいろ");
	menu.root.emplace_back(0x5f4, "設定");
	menu.root.back().emplace_back(0x628, "ログ表示");
	menu.root.back().back().emplace_back(0x320, "戦闘", [](int, Menu::Item& i) {Log::mute[Log::Tag::battle] ^= true; if (Log::mute[Log::Tag::battle]) { i.icon = 0x32a; } else { i.icon = 0x320; }});
	menu.root.back().back().emplace_back(0x320, "システム", [](int, Menu::Item& i) {Log::mute[Log::Tag::system] ^= true; if (Log::mute[Log::Tag::system]) { i.icon = 0x32a; } else { i.icon = 0x320; }});
	menu.root.back().back().emplace_back(0x320, "会話", [](int, Menu::Item& i) {Log::mute[Log::Tag::talk] ^= true; if (Log::mute[Log::Tag::talk]) { i.icon = 0x32a; } else { i.icon = 0x320; }});
	menu.root.back().back().emplace_back(0x320, "デバッグ", [](int, Menu::Item& i) {Log::mute[Log::Tag::debug] ^= true; if (Log::mute[Log::Tag::debug]) { i.icon = 0x32a; } else { i.icon = 0x320; }});
	menu.root.back().emplace_back(0x629, "自動セーブ");
}

void Manager::update()
{
	if (!pause && ++count >= speed)
	{
		count = 0;
		battle.update();
	}
	if (Mouse::click(MOUSE_INPUT_1))
	{
		if (Log::hit(Mouse::pos()))
		{
			textline = 0;
		}
		if (menu.hit(Mouse::pos()))
		{
			menu.select(Mouse::pos());
		}
	}
	if (Mouse::wheel() != 0)
	{
		if (Log::hit(Mouse::pos()))
		{
			textline = __max(__min(textline - Mouse::wheel(), Log::maxNum), 0);
		}
	}
}

void Manager::draw()
{
	Log::draw(textline);
	menu.draw(Mouse::pos());
}

