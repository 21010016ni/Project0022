#pragma once
#include <vector>
#include "Charactor.hpp"
#include "Display.hpp"

class Palette
{
	static Display display;
public:
	static inline std::vector<Charactor> charactor;
	static bool active;

	static void draw(const Point<int>& mouse);
};

