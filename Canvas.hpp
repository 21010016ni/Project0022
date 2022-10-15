#pragma once
#include "Display.hpp"

class Canvas
{

public:
	static Display display;
	static inline int back;

	static void PlayEffect(int id);
	static void draw();
	static bool hit(const Point<int>& t);
};

