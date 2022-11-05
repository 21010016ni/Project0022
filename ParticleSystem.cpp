#include "ParticleSystem.hpp"
#include <random>
#include <DxLib.h>

void ParticleSystem::reset()
{
	list.clear();
}

void ParticleSystem::update()
{
	for (auto i = list.begin(); i != list.end();)
	{
		if (--(*i)->duration <= 0)
		{
			i = list.erase(i);
			continue;
		}
		(*i)->update();
		++i;
	}
}

void ParticleSystem::draw()
{
	for (const auto& i : list)
		i->draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

