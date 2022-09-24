#pragma once
#include <vector>
#include "Battle.hpp"
#include "Menu.hpp"

class Manager
{
	static inline int font;

	static inline int count = 0;

	static inline std::vector<Charactor> charactor;
	static inline Battle battle;
	static Menu menu;

public:
	static inline bool pause = false;
	static inline int speed = 60;
	static inline int textline = 0;

	static void CountReset();

	static void preset();
	static void update();
	static void draw();
};

