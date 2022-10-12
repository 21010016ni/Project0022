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
#include "string_mb_function.hpp"

void Manager::preset()
{
	font = LoadFontDataToHandle("data/font/TTEdit2_3�S�V�b�N.dft");
	Icon::load("data/picture/icon.png", 16);
	Log::SetFont(font);
	Menu::SetFont(font);
	Menu::SetSE(LoadSoundMem("data/se/select.mp3"), LoadSoundMem("data/se/decision.mp3"));
	Canvas::back = LoadGraph("data/picture/back00.jpg");
	//BGM::set("data/bgm/Changeling.mp3");
	//BGM::set("data/bgm/Etude.mp3");
	//BGM::set("data/bgm/�Ȃ񂾂����邢�����̃����c.wav");
	//BGM::set("data/bgm/��.mp3");
	//BGM::set("data/bgm/��΂Ƒz��������.mp3");
	//BGM::set("data/bgm/���R.mp3");

	// Charactor(name,color,hp,atk,def,mag,reg,spd,tec,luc)
	charactor.emplace_back("�A���R��", 0xff0000ff, 300, 20, 10, 20, 0, 10, 0.8f, 0.5f);
	charactor.back().skill.push_back(DataBase::skill[4]);
	charactor.back().word.emplace(Charactor::search, "�u#ffff00�A���[�c�c���́A[�̒��͂ǂ���,�������ĂȂ���]�v");
	charactor.back().word.emplace(Charactor::reaction + 0xffff00, "�u�ւ���|�u����A�܂��c�c����Ȃ炢���v");
	charactor.back().word.emplace(Charactor::reaction + 0xffff00, "���Y|�u��킢�I�@����A�܂��c�c�Ȃ炢�����ǁv");
	charactor.back().word.emplace(Charactor::reaction + 0xffff00, "�X�e�[�L|�u����ȂƂ��Ŗ����ȁc�c���[�A�A������ȁv");
	charactor.back().word.emplace(Charactor::reaction_any, "���C|�u�܂��c�c�ڂ��ڂ����ĂƂ����B�T���L���v");
	charactor.back().word.emplace(Charactor::reaction_any, "�H|�u[���A�����c�c�����ĂȂ�����,������Ƃ킩��˂��ȁc�c���܂�]�v");
	charactor.back().word.emplace(Charactor::reaction_any, "�u�������v");
	charactor.back().word.emplace(Charactor::battle_start, "�u[���āA��邩,�葁���I��点�邼]�v");
	charactor.back().word.emplace(Charactor::skill_prev, "�u[������,�ق�A������,������点]�v");
	charactor.back().word.emplace(Charactor::skill_after, "�u�܁A����Ȃ���v");
	charactor.emplace_back("���v���J", 0xffffff00, 300, 8, 0, 0, 0, 10, 0.8f, 0.5f);
	charactor.back().skill.push_back(DataBase::skill[3]);
	charactor.back().skill.push_back(DataBase::skill[1]);
	charactor.back().word.emplace(Charactor::reaction + 0x0000ff, "�̒��͂ǂ�|�u�ւ����B���肪�ƁA#0000ff�v");
	charactor.back().word.emplace(Charactor::reaction + 0x0000ff, "�������ĂȂ�|�u[�c�c�H�@���������ׂ��B#0000ff�A�����Y�ꂽ�H�@���Y�H,����A�X�e�[�L���H�ׂ����B�p�ӂ��āA#0000ff]�v");
	charactor.back().word.emplace(Charactor::reaction_any, "�H|�u[�ׂɁc�c,����Ȃ�]�v");
	charactor.back().word.emplace(Charactor::reaction_any, "�u[�c�c�ȂɁB���邳��,�͂Ȃ����ƂȂ���]�v");
	charactor.back().word.emplace(Charactor::battle_start, "�u[�c�c�����,��A�o���Ă����H�@�c�c����]�v");
	charactor.emplace_back("�Ȃ񂩂悢�G", 0xffff88ff, 800, 10, 0, 0, 0, 10, 0.2f, 1.4f);
	charactor.back().skill.push_back(DataBase::skill[8]);
	charactor.back().skill.push_back(DataBase::skill[9]);
	charactor.back().word.emplace(Charactor::search, "�u[��l��A�������߂�,�J�n����]�v");
	charactor.back().word.emplace(Charactor::skill_prev, "�u���āA�n�߂悤�v");
	charactor.back().word.emplace(Charactor::skill_prev + 1, "�u<c,0xff4444>�c�c���Y<c>�v");
	charactor.back().word.emplace(Charactor::battle_start, "�u[��l��A�������߂�,�J�n����]�v");

	Field::set(charactor[0], Unit::Faction::player);
	Field::set(charactor[1], Unit::Faction::player);
	Field::set(charactor[2], Unit::Faction::enemy);

	Menu::root.emplace_back(0x338, "�퓬�i�s", [](int, Menu::Item& i) { Field::pause ^= true; Field::CountReset(); if (Field::pause) { i.icon = 0x350; } else { i.icon = 0x338; } });
	Menu::root.emplace_back(0x5e5, "����");
	Menu::root.back().emplace_back(0x621, "�p���b�g�]�X");
	Menu::root.back().emplace_back(0x621, "�퓬�X�s�[�h");
	Menu::root.emplace_back(0x5f7, "����");
	Menu::root.back().emplace_back(0x624, "���낢��");
	Menu::root.emplace_back(0x5f4, "�ݒ�");
	Menu::root.back().emplace_back(0x628, "���O�\��");
	Menu::root.back().back().emplace_back(0x320, "�퓬", [](int, Menu::Item& i) { Log::mute[Log::Tag::battle] ^= true; if(Log::mute[Log::Tag::battle]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, "�V�X�e��", [](int, Menu::Item& i) { Log::mute[Log::Tag::system] ^= true; if(Log::mute[Log::Tag::system]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, "��b", [](int, Menu::Item& i) { Log::mute[Log::Tag::talk] ^= true; if(Log::mute[Log::Tag::talk]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().back().emplace_back(0x320, "�f�o�b�O", [](int, Menu::Item& i) { Log::mute[Log::Tag::debug] ^= true; if(Log::mute[Log::Tag::debug]) { i.icon = 0x32a; } else { i.icon = 0x320; } });
	Menu::root.back().emplace_back(0x629, "�����Z�[�u");

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem("data/se/�����E�a��01.mp3"));

	volume.mute &= 0b11111110;

	BGM::volume = volume.Bgm() ? volume.bgm : 0;
	Effect::volume = volume.Se() ? volume.se : 0;
	Menu::SetSEVolume(volume.Se() ? volume.se : 0);
}

void Manager::update()
{
	Field::update();

	// ���͎�t
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

	// BGM�X�V����
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
		DrawStringToHandle(1020 - GetDrawStringWidthToHandle(BGM::title.c_str(), (int)BGM::title.size(), font), 4, BGM::title.c_str(), 0xfffffff, font);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		BGMCount -= 2;
	}
}

