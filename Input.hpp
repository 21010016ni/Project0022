#pragma once
#include <DxLib.h>
#include "Point.hpp"

class Keyboard
{
	Keyboard() = delete;

protected:
	static inline struct
	{
		unsigned char v = 0;
		void operator<<(char n)noexcept { (v <<= 1) += n; }
		explicit operator unsigned char& ()noexcept { return v; }
		bool press()const noexcept { return v & 1; }
		bool push()const noexcept { return (v & 3) == 1; }
		bool pull()const noexcept { return (v & 3) == 2; }
	} value[256];

public:
	static void update()noexcept
	{
		static char key[256] = { 0 };
		static unsigned char i = 0;
		GetHitKeyStateAll(key);
		do
			value[i] << key[i];
		while (++i != 0);
	}

	static unsigned char& get(unsigned char code)noexcept { return (unsigned char&)value[code]; }
	static bool press(unsigned char code)noexcept { return value[code].press(); }
	static bool push(unsigned char code)noexcept { return value[code].push(); }
	static bool pull(unsigned char code)noexcept { return value[code].pull(); }
};

class Mouse
{
	Mouse() = delete;

protected:
	static inline int _button = 0, _type = -1;
	static inline Point<int> _pos = { -1,-1 };
	static inline bool _update = false;
	static inline int _wheel = 0;

public:
	static bool update()noexcept { return _wheel = GetMouseWheelRotVol(), _update = (GetMouseInputLog2(&_button, &_pos.x, &_pos.y, &_type) ? (GetMousePoint(&_pos.x, &_pos.y), false) : true); }

	static bool change()noexcept { return _update; }
	static bool button(int b)noexcept { return _button & b; }
	static const Point<int>& pos()noexcept { return _pos; }
	static int x()noexcept { return _pos.x; }
	static int y()noexcept { return _pos.y; }
	static bool type()noexcept { return !_type; }
	static int wheel()noexcept { return _wheel; }
	static bool click(int b)noexcept { return _update && _button & b && !_type; }
	static bool releace(int b)noexcept { return _update && _button & b && _type; }
};

