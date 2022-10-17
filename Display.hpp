#pragma once
#include <DxLib.h>
#include <string>
#include "Point.hpp"

enum Ref :unsigned char
{
	left = 0b00000000,
	center = 0b00000001,
	right = 0b00000010,
	top = 0b00000000,
	middle = 0b00000100,
	under = 0b00001000,
};

class Display
{
	static inline int font = -1;
	static inline int fontSize = 0;

public:
	Point<int> pos;
	Point<int> siz;

	Display(const Point<int>& pos, const Point<int>& siz);
	Display(const Point<int>& pos, const Point<int>& siz, int font);

	static void SetFont(int fontHandle)
	{
		font = fontHandle;
		GetFontStateToHandle(0, &fontSize, 0, font);
	}
	static const int GetFontSize()
	{
		return fontSize;
	}

	Point<int> localize(const Point<int>& t)const { return t - pos; }
	bool hit(const Point<int>& t)const { return pos.y <= t.y && t.y < pos.y + siz.y && pos.x <= t.x && t.x < pos.x + siz.x; }

	void lock()const
	{
		DxLib::SetDrawArea(pos.x, pos.y, pos.x + siz.x, pos.y + siz.y);
	}
	void unlock()const
	{
		DxLib::SetDrawAreaFull();
	}
	void DrawBox(const Point<int>& dst, const Point<int>& s, unsigned int color, bool fill)const
	{
		DxLib::DrawBox(pos.x + dst.x, pos.y + dst.y, pos.x + dst.x + s.x, pos.y + dst.y + dst.y, color, fill);
	}
	void DrawBox(int x, int y, const Point<int>& s, unsigned int color, bool fill)const
	{
		DxLib::DrawBox(pos.x + x, pos.y + y, pos.x + x + s.x, pos.y + y + s.y, color, fill);
	}
	void DrawCircle(const Point<int>& dst, int r, unsigned int color, bool fill)const
	{
		DxLib::DrawCircle(pos.x + dst.x, pos.y + dst.y, r, color, fill);
	}
	void DrawCircle(int x, int y, int r, unsigned int color, bool fill)const
	{
		DxLib::DrawCircle(pos.x + x, pos.y + y, r, color, fill);
	}
	void DrawGraph(const Point<int>& dst, int handle, bool TransFlag = true)
	{
		DxLib::DrawGraph(pos.x+dst.x, pos.y + dst.y, handle, TransFlag);
	}
	void DrawGraph(int x, int y, int handle, bool TransFlag = true)
	{
		DxLib::DrawGraph(pos.x + x, pos.y + y, handle, TransFlag);
	}
	void DrawIcon(const Point<int>& dst, int id);
	void DrawIcon(int x, int y, int id);
	void DrawString(const Point<int>& dst, const std::u8string& text, unsigned int color)const;
	void DrawString(int x, int y, const std::u8string& text, unsigned int color)const;
	void DrawString(const Point<int>& dst, const std::u8string& text, unsigned int color, int font)const;
	void DrawString(int x, int y, const std::u8string& text, unsigned int color, int font)const;
	void DrawRawString(const Point<int>& dst, const std::u8string& text, unsigned int color, unsigned char ref = 0)const;
	void DrawRawString(int x, int y, const std::u8string& text, unsigned int color, unsigned char ref = 0)const;
};

