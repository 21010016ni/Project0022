#pragma once
#include <DxLib.h>
#include "Particle.hpp"
#include "Point.hpp"

class Dust :public Particle
{
	Point<float> pos;
	Point<float> vec;
	unsigned int color;

	void update()override
	{
		pos += vec;
		vec.rotate<float>(std::uniform_real_distribution{-0.2f,0.2f}(mt));
	}
	void draw()const override
	{
		if(duration > 60)
			DrawCircle((int)pos.x, (int)pos.y, 1, color);
		DrawCircle((int)pos.x, (int)pos.y, 2, color);
	}

public:
	Dust() :Particle(RangeI{200,800}(mt)), pos(RangeF{0,600}(mt), RangeF{0,1024}(mt)), vec(RangeF{-0.5f,0.5f}(mt), RangeF{-0.5f,0.5f}(mt)), color(mt())
	{
	}
};

