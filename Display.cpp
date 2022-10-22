#include "Display.hpp"
#include <boost/algorithm/string.hpp>
#include "Icon.hpp"
#include "convert_string.hpp"

constexpr float PI = 3.1415926535897932384626433832795028841971f;
extern std::mt19937 engine;

void Display::Shake::set(int l, int t, float p)
{
	count = 0;
	level = l;
	time = t;
	power = p;
	dir = {0.0f,0.0f};
	value = {0,0};
}
void Display::Shake::set(int l, int t, const Point<float>& d)
{
	count = 0;
	level = l;
	time = t;
	power = 0.0f;
	dir = d;
	value = {0,0};
}
bool Display::Shake::update()
{
	if(time != 0 && count < time)
	{
		++count;
		if(power == 0.0f)
			value = static_cast<Point<int>>(dir * std::sin(PI * 2 * count / time));
		else
			value = static_cast<Point<int>>(Point<float>{range(engine), range(engine)} *power);
		return true;
	}
	else
	{
		value = {0,0};
		return false;
	}
}
Point<int> Display::Shake::get(int l)const
{
	if(l <= level)
		return value;
	else
		return {0,0};
}
int Display::Shake::y(int l)const
{
	if(l <= level)
		return -value.y;
	else
		return 0;
}
int Display::Shake::x(int l)const
{
	if(l <= level)
		return value.x;
	else
		return 0;
}

void Display::DrawIcon(const Point<int>& dst, int id)
{
	Icon::draw(pos + dst + shake.get(level), id);
}
void Display::DrawIcon(int x, int y, int id)
{
	Icon::draw(pos.x + x + shake.x(level), pos.y + y + shake.y(level), id);
}

void Display::DrawString(int x, int y, const std::u8string& text, unsigned int color)const
{
	DrawString({ y,x }, text, color, font);
}
void Display::DrawString(int x, int y, const std::u8string& text, unsigned int color, int font)const
{
	DrawString({ y,x }, text, color, font);
}
void Display::DrawString(const Point<int>& dst, const std::u8string& text, unsigned int color) const
{
	DrawString(dst, text, color, font);
}
void Display::DrawString(const Point<int>& dst, const std::u8string& text, unsigned int color, int font)const
{
	unsigned int c = color;
	static std::vector<std::u8string> elem;
	static std::u8string buf;
	size_t tp = 0, prev;
	Point<int> textCursor = pos + dst;
	// ループ
	while (tp < text.size())
	{
		prev = tp;
		if (text[tp] == '<')
		{
			if ((tp = text.find_first_of('>', tp)) == std::u8string::npos)
				throw std::invalid_argument("'>' is not found.");
			boost::split(elem, text.substr(++prev, tp - prev - 1), boost::is_any_of(","));
			++tp;

			// 特殊文字処理
			try
			{
				if (elem[0].empty())
				{
					// もし処理の値が未設定だったら、<を出力して終了
					DxLib::DrawStringToHandle(textCursor.x + shake.x(level), textCursor.y + shake.y(level), "<", c, font);
					textCursor.x += GetDrawStringWidthToHandle("<", 1, font);
				}
				else if (elem[0] == u8"br")
				{
					// n 改行
					textCursor.y += fontSize;
					textCursor.x = dst.x + pos.x;
				}
				else if (elem[0] == u8"i")
				{
					// i 指定した番号のアイコンを表示
					Icon::draw(textCursor + shake.get(level), std::stoi(ext::convert(elem.at(1)), nullptr, 16));
					textCursor.x += Icon::get_size();
				}
				else if (elem[0] == u8"c")
				{
					if (elem.size() == 1 || elem[1] == u8"reset")
						c = color;
					else
						c = std::stoi(ext::convert(elem[1]), nullptr, 0);
				}
			}
			catch (std::out_of_range)
			{
				throw std::invalid_argument("invalid argument.");
			}
		}
		else if (text[tp] == '#')
		{
			DxLib::DrawCircle(textCursor.x + fontSize / 2 - 1 + shake.x(level), textCursor.y + (fontSize + 2) / 2 + shake.y(level), fontSize / 2 - 1, std::stoi(ext::convert(text.substr(tp + 1, 6)), nullptr, 16));
			DxLib::DrawCircle(textCursor.x + fontSize / 2 - 1 + shake.x(level), textCursor.y + (fontSize + 2) / 2 + shake.y(level), fontSize / 2 - 1, 0xff888888, FALSE);
			tp += 7;
			textCursor.x += fontSize;
		}
		else
		{
			// 通常表示処理
			tp = text.find_first_of(u8"<#", tp);
			buf = text.substr(prev, tp - prev);
			DxLib::DrawStringToHandle(textCursor.x + shake.x(level), textCursor.y + shake.y(level), ext::tochar(buf), c, font);
			if (int i = (int)std::count(buf.cbegin(), buf.cend(), '\n'))
			{
				textCursor.y += fontSize * i;
				textCursor.x = dst.x + pos.x;
			}
			else
				textCursor.x += GetDrawStringWidthToHandle(ext::tochar(buf), (int)buf.size(), font);
		}
	}
}

void Display::DrawRawString(const Point<int>& dst, const std::u8string& text, unsigned int color, unsigned char ref) const
{
	DrawRawString(dst.x, dst.y, text, color, ref);
}
void Display::DrawRawString(int x, int y, const std::u8string& text, unsigned int color, unsigned char ref) const
{
	int rx = 0, ry = 0;
	if (ref & 0b00000011)
	{
		rx = GetDrawStringWidthToHandle(ext::tochar(text), (int)text.size(), font);
		if (ref & 0b00000001)
			rx /= 2;
	}
	if (ref & 0b00001100)
	{
		ry = fontSize;
		if (ref & 0b00000100)
			ry /= 2;
	}
	DxLib::DrawStringToHandle(pos.x + x - rx + shake.x(level), pos.y + y - ry + shake.y(level), ext::tochar(text), color, font);
}

