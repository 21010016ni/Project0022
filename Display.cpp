#include "Display.hpp"
#include <boost/algorithm/string.hpp>
#include "Icon.hpp"
#include "convert_string.hpp"

Display::Display(const Point<int>& pos, const Point<int>& siz) :pos(pos), siz(siz), font(-1), fontSize(0)
{
}
Display::Display(const Point<int>& pos, const Point<int>& siz, int font) :pos(pos), siz(siz), font(font)
{
	GetFontStateToHandle(0, &fontSize, 0, font);
}

void Display::DrawIcon(const Point<int>& dst, int id)
{
	Icon::draw(pos + dst, id);
}
void Display::DrawIcon(int x, int y, int id)
{
	Icon::draw(pos.x + x, pos.y + y, id);
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
	// ���[�v
	while (tp < text.size())
	{
		prev = tp;
		if (text[tp] == '<')
		{
			if ((tp = text.find_first_of('>', tp)) == std::u8string::npos)
				throw std::invalid_argument("'>' is not found.");
			boost::split(elem, text.substr(++prev, tp - prev - 1), boost::is_any_of(","));
			++tp;

			// ���ꕶ������
			try
			{
				if (elem[0].empty())
				{
					// ���������̒l�����ݒ肾������A<���o�͂��ďI��
					DxLib::DrawStringToHandle(textCursor.x, textCursor.y, "<", c, font);
					textCursor.x += GetDrawStringWidthToHandle("<", 1, font);
				}
				else if (elem[0] == u8"n")
				{
					// n ���s
					textCursor.y += fontSize;
					textCursor.x = dst.x + pos.x;
				}
				else if (elem[0] == u8"i")
				{
					// i �w�肵���ԍ��̃A�C�R����\��
					Icon::draw(textCursor, std::stoi(ext::convert(elem.at(1)), nullptr, 16));
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
			DxLib::DrawCircle(textCursor.x + (fontSize - 4) / 2 + 1, textCursor.y + (fontSize - 4) / 2 + 1, (fontSize - 4) / 2, std::stoi(ext::convert(text.substr(tp + 1, 6)), nullptr, 16));
			DxLib::DrawCircle(textCursor.x + (fontSize - 4) / 2 + 1, textCursor.y + (fontSize - 4) / 2 + 1, (fontSize - 4) / 2, 0xff888888, FALSE);
			tp += 7;
			textCursor.x += fontSize;
		}
		else
		{
			// �ʏ�\������
			tp = text.find_first_of(u8"<#", tp);
			buf = text.substr(prev, tp - prev);
			DxLib::DrawStringToHandle(textCursor.x, textCursor.y, ext::tochar(buf), c, font);
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

void Display::DrawRawString(const Point<int>& dst, const std::u8string& text, unsigned int color) const
{
	DxLib::DrawStringToHandle(pos.x + dst.x, pos.y + dst.y, ext::tochar(text), color, font);
}
void Display::DrawRawString(int x, int y, const std::u8string& text, unsigned int color) const
{
	DxLib::DrawStringToHandle(pos.x + x, pos.y + y, ext::tochar(text), color, font);
}
void Display::DrawRawString(const Point<int>& dst, const std::u8string& text, unsigned int color, int font) const
{
	DxLib::DrawStringToHandle(pos.x + dst.x, pos.y + dst.y, ext::tochar(text), color, font);
}
void Display::DrawRawString(int x, int y, const std::u8string& text, unsigned int color, int font) const
{
	DxLib::DrawStringToHandle(pos.x + x, pos.y + y, ext::tochar(text), color, font);
}

