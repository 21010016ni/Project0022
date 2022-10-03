#pragma once
#include "Display.hpp"

class Canvas
{
	static Display display;

public:
	static inline int back;

	static void PlayEffect(int id);
	static void draw();
};

