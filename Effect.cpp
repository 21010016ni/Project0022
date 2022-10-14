#include "Effect.hpp"
#include <DxLib.h>

Effect::Data::Data(int graph, int wn, int hn, int se, unsigned char volume) :graph(graph), num(hn, wn), se(se), volume(volume)
{
	GetGraphSize(graph, &size.x, &size.y);
	size /= num;
}

void Effect::Data::draw(int x, int y, int t)const
{
	DrawRectGraph(x, y, (t % num.x) * size.x, (t / num.x) * size.y, size.x, size.y, graph, TRUE);
}

void Effect::Data::play()const
{
	PlaySoundMem(se, DX_PLAYTYPE_BACK);
}

Effect::Inst::Inst(const Data* data, int x, int y, Pos pos) :data(data), dst(y, x), time(0), pos(pos)
{

}

void Effect::Inst::play()const
{
	if(time == 0)
		data->play();
	switch(pos)
	{
	case Pos::leftup:
		data->draw(dst.x, dst.y, time);
		break;
	case Pos::rightup:
		data->draw(dst.x - data->size.x, dst.y, time);
		break;
	case Pos::leftdown:
		data->draw(dst.x, dst.y - data->size.y, time);
		break;
	case Pos::rightdown:
		data->draw(dst.x - data->size.x, dst.y - data->size.y, time);
		break;
	case Pos::center:
		data->draw(dst.x - data->size.x / 2, dst.y - data->size.y / 2, time);
		break;
	}
}

void Effect::load(const std::u8string& FileName)
{

}

void Effect::load(int graph, int wn, int hn, int se)
{
	data.emplace_back(graph, wn, hn, se, volume);
}

void Effect::set(int id, int x, int y, Pos pos)
{
	list.emplace_back(&data.at(id), x, y, pos);
}

void Effect::play()
{
	for(auto i = list.begin(); i != list.end();)
	{
		if(i->data->volume != volume)
		{
			ChangeVolumeSoundMem(volume, i->data->se);
			i->data->volume = volume;
		}
		i->play();
		if(++(i->time) >= i->data->size.y * i->data->size.x)
		{
			list.erase(i);
			continue;
		}
		++i;
	}
}

