#pragma once
#include <DxLib.h>
#include "Point.hpp"

class Keyboard
{
	Keyboard() = delete;

protected:
	struct Value
	{
		BYTE v = 0;
		void operator<<(BYTE n)noexcept { (v <<= 1) += n; }
		explicit operator BYTE& ()noexcept { return v; }
		bool press()const noexcept { return v & 1; }
		bool push()const noexcept { return (v & 3) == 1; }
		bool pull()const noexcept { return (v & 3) == 2; }
	};
	static inline Value value[256];

public:
	static void update()noexcept
	{
		static BYTE key[256] = {0};
		static BYTE i = 0;
		(void)GetKeyboardState(key);
		do
			value[i] << bool(key[i] & 0x80);
		while (++i != 0);
	}

	static BYTE& get(BYTE code)noexcept { return (BYTE&)value[code]; }
	static bool press(BYTE code)noexcept { return value[code].press(); }
	static bool push(BYTE code)noexcept { return value[code].push(); }
	static bool pull(BYTE code)noexcept { return value[code].pull(); }
	static bool press()
	{
		for (BYTE i = 0; i < 240; ++i)
			if (press(i))
				return true;
		return false;
	}
	static bool push()
	{
		for (BYTE i = 0; i < 240; ++i)
			if (push(i))
				return true;
		return false;
	}
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

