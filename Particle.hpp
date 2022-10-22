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

