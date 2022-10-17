#include "Manager.hpp"
#include <DxLib.h>
#include "Particle.hpp"
#include "Icon.hpp"
#include "Log.hpp"
#include "DataBase.hpp"
#include "BGM.hpp"
#include "Field.hpp"
#include "Input.hpp"
#include "Effect.hpp"
#include "Canvas.hpp"
#include "Palette.hpp"
#include "HandleManager.hpp"
#include "convert_string.hpp"

void Manager::preset()
{
	font = LoadFontDataToHandle((const char*)u8"data/font/NotoSansJPLight.dft");
	Icon::load("data/picture/icon.png", 16);
	Display::SetFont(font);
	Menu::SetSE(LoadSoundMem("data/se/select.mp3"), LoadSoundMem("data/se/decision.mp3"));
	Canvas::back = LoadGraph("data/picture/back00.jpg");
	ChangeFont("Noto Sans JP Light");
	//BGM::set("data/bgm/Changeling.mp3");
	//BGM::set("data/bgm/Etude.mp3");
	//BGM::set("data/bgm/なんだか明るい感じのワルツ.wav");
	//BGM::set("data/bgm/夢.mp3");
	//BGM::set("data/bgm/宝石と想いを胸に.mp3");
	//BGM::set("data/bgm/自然.mp3");

	// Charactor(name,color,hp,atk,def,mag,reg,spd,tec,luc)
	//charactor.emplace_back(u8"キャラクター名", カラーコード, HP, ATK, DEF, MAG, REG, SPD, TEC, LUC);
	//charactor.back().skill.emplace_back(スキルID, エフェクトID);
	//charactor.back().word.emplace(台詞キー, u8"内容");

	Palette::charactor.emplace_back(Charactor::load("data/charactor/0000ff.bin"));
	Palette::charactor.back().graph.icon = u8"data/charactor/robot_icon.png";
	Palette::charactor.emplace_back(Charactor::load("data/charactor/ffff00.bin"));
	Palette::charactor.back().graph.icon = u8"data/charactor/robot_icon.png";
	Palette::charactor.emplace_back(Charactor::load("data/charactor/ff88ff.bin"));
	Palette::charactor.back().graph.icon = u8"data/charactor/robot_icon.png";

	Field::set(Palette::charactor[0], Unit::Faction::player);
	Field::set(Palette::charactor[1], Unit::Faction::player);
	Field::set(Palette::charactor[2], Unit::Faction::enemy);

	Menu::root.emplace_back(0x338, u8"戦闘進行", [](int, Menu::Item& i) { Field::pause ^= true; Field::CountReset(); if (Field::pause) { i.icon = 0x350; } else { i.icon = 0x338; } });
	Menu::root.emplace_back(0x37f, u8"戦闘スピード");
	Menu::root.emplace_back(0x5f9, u8"パレット", [](int, Menu::Item&) {Palette::active ^= true; });
	Menu::root.emplace_back(0x5f7, u8"実績");
	Menu::root.back().emplace_back(0x624, u8"いろいろ");
	Menu::root.emplace_back(0x5f4, u8"設定");
	Menu::root.back().emplace_back(0x628, u8"ログ表示");
	Menu::root.back().back().emplace_back(0x320, u8"会話", [](int, Menu::Item& i) { Log::mute[Log::Tag::talk] ^= true; if (Log::mute[Log::Tag::talk]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, u8"戦闘", [](int, Menu::Item& i) { Log::mute[Log::Tag::battle] ^= true; if(Log::mute[Log::Tag::battle]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, u8"システム", [](int, Menu::Item& i) { Log::mute[Log::Tag::system] ^= true; if(Log::mute[Log::Tag::system]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, u8"デバッグ", [](int, Menu::Item& i) { Log::mute[Log::Tag::debug] ^= true; if(Log::mute[Log::Tag::debug]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().emplace_back(0x629, u8"自動セーブ");

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem((const char*)u8"data/se/刀剣・斬る01.mp3"));

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
	if (Mouse::type() && Mouse::button(MOUSE_INPUT_2))
	{
		if (Menu::hit(Mouse::pos()))
		{
			Menu::display.pos = Mouse::pos() - Menu::display.siz / 2;
		}
		else if (Log::hit(Mouse::pos()))
		{
			Log::display.pos = Mouse::pos() - Log::display.siz / 2;
		}
		else if (Canvas::hit(Mouse::pos()))
		{
			Canvas::display.pos = Mouse::pos() - Canvas::display.siz / 2;
		}
	}

	// BGM更新処理
	if(BGM::update())
	{
		BGMCount = 600;
	}
	// ハンドル管理更新
	HandleManager::update();
}

void Manager::draw()
{
	Canvas::draw();
	Log::draw(textline);
	Menu::draw(Mouse::pos());
	Palette::draw(Mouse::pos());
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

	ParticleSystem::draw();
}

