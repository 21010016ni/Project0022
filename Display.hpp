#pragma once
#include <DxLib.h>
#include <string>
#include <random>
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

	class Shake
	{
		int count;
		std::uniform_real_distribution<float> range;

		int level;
		int time;
		float power;
		Point<float> dir;
		Point<int> value;

	public:
		Shake() :count(0), range(-1.0f, 1.0f), level(0), time(0), power(0.0f) {}

		// 毎フレームランダム
		void set(int level, int time, float power);
		// 方向を指定して正弦波1回
		void set(int level, int time, const Point<float>& dir);
		// 振動量を更新
		bool update();
		// 振動量を取得
		Point<int> get(int level)const;
		int y(int level)const;
		int x(int level)const;
	};

public:
	static inline Shake shake;

	Point<int> pos;
	Point<int> siz;
	int level;

	Display(const Point<int>& pos, const Point<int>& siz, int level) :pos(pos), siz(siz), level(level) {};

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
		DxLib::SetDrawArea(pos.x + shake.x(level), pos.y + shake.y(level), pos.x + siz.x + shake.x(level), pos.y + siz.y + shake.y(level));
	}
	void unlock()const
	{
		DxLib::SetDrawAreaFull();
	}
	void DrawBox(const Point<int>& dst, const Point<int>& s, unsigned int color, bool fill)const
	{
		DxLib::DrawBox(pos.x + dst.x + shake.x(level), pos.y + dst.y + shake.y(level), pos.x + dst.x + s.x + shake.x(level), pos.y + dst.y + dst.y + shake.y(level), color, fill);
	}
	void DrawBox(int x, int y, const Point<int>& s, unsigned int color, bool fill)const
	{
		DxLib::DrawBox(pos.x + x + shake.x(level), pos.y + y + shake.y(level), pos.x + x + s.x + shake.x(level), pos.y + y + s.y + shake.y(level), color, fill);
	}
	void DrawCircle(const Point<int>& dst, int r, unsigned int color, bool fill)const
	{
		DxLib::DrawCircle(pos.x + dst.x + shake.x(level), pos.y + dst.y + shake.y(level), r, color, fill);
	}
	void DrawCircle(int x, int y, int r, unsigned int color, bool fill)const
	{
		DxLib::DrawCircle(pos.x + x + shake.x(level), pos.y + y + shake.y(level), r, color, fill);
	}
	void DrawGraph(const Point<int>& dst, int handle, bool TransFlag = true)
	{
		DxLib::DrawGraph(pos.x + dst.x + shake.x(level), pos.y + dst.y + shake.y(level), handle, TransFlag);
	}
	void DrawGraph(int x, int y, int handle, bool TransFlag = true)
	{
		DxLib::DrawGraph(pos.x + x + shake.x(level), pos.y + y + shake.y(level), handle, TransFlag);
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

