#include "Palette.hpp"
#include <format>
#include "HandleManager.hpp"
#include "convert_string.hpp"

Display Palette::display({0,0}, {480,640}, 1);
bool Palette::active = false;

void Palette::draw(const Point<int>& mouse)
{
	if (!active)
		return;

	auto lm = display.localize(mouse);
	Point<int> boxSize(20, 100);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	display.DrawBox(0, 0, display.siz, 0xff000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < (int)charactor.size(); ++i)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		display.DrawBox(0, i * boxSize.y, boxSize, charactor[i].status.color, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		display.DrawRawString(0, i * boxSize.y + 3, charactor[i].name, 0xffffffff);
	}
	if (lm.y >= 0 && lm.x >= 0 && lm.x < 100)
	{
		int select = lm.y / boxSize.y;
		if (select < charactor.size())
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
			display.DrawBox(0, select * boxSize.y, boxSize, 0x888888, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			display.DrawRawString(boxSize.x + 20, 0, charactor[select].name, 0xffffffff);
			display.DrawRawString(boxSize.x + 300, 0, color(charactor[select].status.color), 0xffffffff);
			display.DrawRawString(boxSize.x + 50, 20, u8"HP :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 50, 33, u8"ATK :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 50, 46, u8"DEF :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 50, 59, u8"MAG :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 50, 72, u8"REG :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 50, 85, u8"SPD :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 50, 98, u8"TEC :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 50, 111, u8"LUC :", 0xffffffff, Ref::right);
			display.DrawRawString(boxSize.x + 55, 20, ext::convert(std::format("{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}", charactor[select].status.hp, charactor[select].status.atk, charactor[select].status.def, charactor[select].status.mag, charactor[select].status.reg, charactor[select].status.spd, charactor[select].status.tec, charactor[select].status.luc)), 0xffffffff);
			display.DrawGraph(boxSize.x + 100, 20, HandleManager::get(charactor[select].graph.icon, HandleManager::Type::graph), true);
		}
	}
}

