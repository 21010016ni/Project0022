#pragma once
#include <vector>
#include "Battle.hpp"
#include "Menu.hpp"

class Manager
{
	struct Volume
	{
		float master;
		unsigned char bgm;
		unsigned char se;
		unsigned char mute;
		Volume() :master(1.0f), bgm(255), se(255), mute(0xff) {}
		bool Master() { return mute & 0x01; }
		bool Bgm() { return mute & 0x01 && mute & 0x02; }
		bool Se() { return mute & 0x01 && mute & 0x04; }
	};

	static inline int font;

	static inline int count = 0;

	static inline int BGMCount = 0;

	static inline std::vector<Charactor> charactor;
	static inline Battle battle;

public:
	static inline bool pause = false;
	static inline int speed = 60;
	static inline int textline = 0;

	static inline Volume volume;

	static void CountReset();

	static void preset();
	static void update();
	static void draw();
};

