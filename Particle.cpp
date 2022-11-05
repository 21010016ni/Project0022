#include "Particle.hpp"
#include <DxLib.h>
#include "Display.hpp"

extern std::mt19937 engine;

Dust::Dust() :Particle(rangeI{ 200,800 }(engine)), pos(rangeF{ 0,600 }(engine), rangeF{ 0,1024 }(engine)), vec(rangeF{ -0.1f,0.1f }(engine), rangeF{ -0.3f,0.3f }(engine)), alpha(rangeI{ 256,1024 }(engine))
{
}

void Dust::update()
{
	vec.rotate<float>(std::uniform_real_distribution{ -0.2f,0.2f }(engine));
	pos += vec;
	alpha -= rangeI{ 0,1 }(engine);
	alpha = __max(alpha, 128);
}

void Dust::draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha / 6);
	DrawCircleAA(pos.x + Display::shake.x(0), pos.y + Display::shake.y(0), alpha / 512.0f + 0.5f, 5, 0xffffffff, TRUE);
}

