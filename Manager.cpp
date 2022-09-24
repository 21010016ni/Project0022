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
	Log::display.SetFont(font);
	menu.SetFont(font);
	Icon::load("data/picture/icon.png", 16);

	charactor.emplace_back("テスト1", 0xffffffff, 30, 5);
	charactor.back().skill.push_back(DataBase::skill[4]);
	charactor.emplace_back("テスト2", 0xffffffff, 300, 8);
	charactor.back().skill.push_back(DataBase::skill[3]);
	charactor.back().skill.push_back(DataBase::skill[1]);
	charactor.emplace_back("テスト3", 0xffffffff, 800, 10);
	charactor.back().skill.push_back(DataBase::skill[8]);
	charactor.back().skill.push_back(DataBase::skill[2]);

	battle.set(charactor[0], Unit::Faction::player);
	battle.set(charactor[1], Unit::Faction::player);
	battle.set(charactor[2], Unit::Faction::enemy);

	menu.root.emplace_back(0x5e5, "操作");
	menu.root.back().emplace_back(0x621, "パレット云々");
	menu.root.back().emplace_back(0x621, "戦闘スピード");
	menu.root.back().emplace_back(0x621, "一時停止", [](int) {Manager::pause ^= true; Manager::CountReset(); });
	menu.root.emplace_back(0x5f7, "実績");
	menu.root.back().emplace_back(0x624, "いろいろ");
	menu.root.emplace_back(0x5f4, "設定");
	menu.root.back().emplace_back(0x628, "ログ表示");
	menu.root.back().back().emplace_back(0x628, "戦闘", [](int) {Log::mute[Log::Tag::battle] ^= true; });
	menu.root.back().back().emplace_back(0x628, "システム", [](int) {Log::mute[Log::Tag::system] ^= true; });
	menu.root.back().back().emplace_back(0x628, "会話", [](int) {Log::mute[Log::Tag::talk] ^= true; });
	menu.root.back().back().emplace_back(0x628, "デバッグ", [](int) {Log::mute[Log::Tag::debug] ^= true; });
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
		menu.select(Mouse::pos());
	}
}

void Manager::draw()
{
	Log::draw(0);
	menu.draw(Mouse::pos());
}

