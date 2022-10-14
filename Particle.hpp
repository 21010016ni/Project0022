#pragma once
#include <memory>
#include <random>
#include <list>

static std::mt19937 mt{ std::random_device{}() };

class ParticleSystem
{
public:
	class Particle
	{
		friend ParticleSystem;
		virtual void update() = 0;
		virtual void draw()const = 0;

	protected:
		int duration;
		Particle(int duration = 0) :duration(duration) {}
	};

private:
	static inline std::list<std::unique_ptr<Particle>> list;

public:
	template <class T> static void add(size_t num = 1)
	{
		while (num-- > 0)
		{
			list.emplace_back(new T);
		}
	}
	static void update();
	static void draw();
};

using Particle = ParticleSystem::Particle;
using RangeI = std::uniform_int_distribution<int>;
using RangeF = std::uniform_real_distribution<float>;

#include <DxLib.h>
#include "Point.hpp"
class Dust :public Particle
{
	Point<float> pos;
	Point<float> vec;
	unsigned int color;

	void update()override
	{
		pos += vec;
		vec.rotate<float>(std::uniform_real_distribution{ -0.2f,0.2f }(mt));
	}
	void draw()const override
	{
		if (duration > 60)
			DrawCircle((int)pos.x, (int)pos.y, 1, color);
		DrawCircle((int)pos.x, (int)pos.y, 2, color);
	}

public:
	Dust() :Particle(RangeI{ 200,800 }(mt)), pos(RangeF{ 0,600 }(mt), RangeF{ 0,1024 }(mt)), vec(RangeF{ -0.5f,0.5f }(mt), RangeF{ -0.5f,0.5f }(mt)), color(mt())
	{
	}
};

