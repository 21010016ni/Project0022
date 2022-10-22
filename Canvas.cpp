#include "Canvas.hpp"
#include <DxLib.h>
#include "Effect.hpp"

Display Canvas::display({0,0}, {480,640}, 0);

void Canvas::PlayEffect(int id)
{
	Effect::set(id, display.pos.x + display.siz.x / 2, display.pos.y + display.siz.y / 2, Effect::Pos::center);
}

void Canvas::draw()
{
	DrawGraph(display.pos.x, display.pos.y, back, TRUE);
}

bool Canvas::hit(const Point<int>& t)
{
	return display.hit(t);
}

