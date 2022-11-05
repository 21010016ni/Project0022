#pragma once
#include <memory>
#include <random>
#include <list>

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
	static void reset();
	static void update();
	static void draw();
};

using Particle = ParticleSystem::Particle;
using rangeI = std::uniform_int_distribution<int>;
using rangeF = std::uniform_real_distribution<float>;

