#include "Log.hpp"

Display Log::display({0,384}, {600,640});

void Log::draw(size_t start)
{
	Point<int> pos(display.siz.y, 12);
	for(size_t i = 0, j = start; i < drawNum && j < text.size(); ++j)
	{
		if(!mute[text[j].tag])
		{
			pos.y -= 24;
			display.DrawString(pos, text[j].value, 0xffffffff);
			if(pos.y < 0)
				break;
			++i;
		}
	}
}

