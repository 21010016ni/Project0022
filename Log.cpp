#include "Log.hpp"
#include <format>
#include <stdexcept>
#include "convert_string.hpp"

Display Log::display({0,384}, {600,640});

void Log::draw(size_t start)
{
	Point<int> pos(display.siz.y, 12);
	for(size_t i = 0, j = start; i < drawNum && j < text.size(); ++j)
	{
		if(!mute[text[j].tag()])
		{
			pos.y -= 24;
			display.DrawString(pos, text[j].text(), 0xffffffff);
			if(pos.y < 0)
				break;
			++i;
		}
	}
}

unsigned int color(const std::u8string& u8)
{
	if (u8[0] != u8'#')
		throw std::invalid_argument("argument not colorcode");
	return std::stoi(ext::convert(u8).substr(1, 6), nullptr, 16);
}

std::u8string color(unsigned int value)
{
	return ext::convert(std::format("#{:06x}", value & 0x00ffffff));
}

bool Log::Text::HasSpeaker()const
{
	if (value->second[0] == '#')
		return true;
	return false;
}

