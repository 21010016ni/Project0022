#pragma once
#include "ParticleSystem.hpp"
#include "Point.hpp"

class Dust :public Particle
{
	Point<float> pos;
	Point<float> vec;
	int alpha;

	void update()override;
	void draw()const override;

public:
	Dust();
};

