#include "Display.hpp"
#include <boost/algorithm/string.hpp>
#include "Icon.hpp"

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
void Display::DrawString(int x, int y, const std::string& text, unsigned int color)const
{
	DrawString({y,x}, text, color, font);
}
void Display::DrawString(int x, int y, const std::string& text, unsigned int color, int font)const
{
	DrawString({y,x}, text, color, font);
}
void Display::DrawString(const Point<int>& dst, const std::string& text, unsigned int color) const
{
	DrawString(dst, text, color, font);
}
void Display::DrawString(const Point<int>& dst, const std::string& text, unsigned int color, int font)const
{
	unsigned int c = color;
	static std::vector<std::string> elem;
	static std::string buf;
	size_t tp = 0, prev;
	Point<int> textCursor = pos + dst;
	// ループ
	while(tp < text.size())
	{
		prev = tp;
		if(text[tp] != '<')
		{
			// 通常表示処理
			tp = text.find_first_of('<', tp);
			buf = text.substr(prev, tp - prev);
			DxLib::DrawStringToHandle(textCursor.x, textCursor.y, buf.c_str(), c, font);
			if(int i = (int)std::count(buf.cbegin(), buf.cend(), '\n'))
			{
				textCursor.y += fontSize * i;
				textCursor.x = dst.x + pos.x;
			}
			else
				textCursor.x += GetDrawStringWidthToHandle(buf.c_str(), (int)buf.size(), font);
		}
		else
		{
			if((tp = text.find_first_of('>', tp)) == std::string::npos)
				throw std::invalid_argument("'>' is not found.");
			boost::split(elem, text.substr(++prev, tp - prev - 1), boost::is_any_of(","));
			++tp;

			// 特殊文字処理
			try
			{
				if(elem[0].empty())
				{
					// もし処理の値が未設定だったら、<を出力して終了
					DxLib::DrawStringToHandle(textCursor.x, textCursor.y, "<", c, font);
					textCursor.x += GetDrawStringWidthToHandle("<", 1, font);
				}
				else if(elem[0] == "n")
				{
					// n 改行
					textCursor.y += fontSize;
					textCursor.x = dst.x + pos.x;
				}
				else if(elem[0] == "i")
				{
					// i 指定した番号のアイコンを表示
					Icon::draw(textCursor, std::stoi(elem.at(1), nullptr, 16));
					textCursor.x += Icon::get_size();
				}
				else if (elem[0] == "c")
				{
					if (elem.size() == 1 || elem[1] == "reset")
						c = color;
					else
						c = std::stoi(elem[1], nullptr, 0);
				}
			}
			catch(std::out_of_range)
			{
				throw std::invalid_argument("invalid argument.");
			}
		}
	}
}

void Display::DrawRawString(const Point<int>& dst, const std::string& text, unsigned int color) const
{
	DxLib::DrawStringToHandle(pos.x + dst.x, pos.y + dst.y, text.c_str(), color, font);
}
void Display::DrawRawString(int x,int y, const std::string& text, unsigned int color) const
{
	DxLib::DrawStringToHandle(pos.x + x, pos.y + y, text.c_str(), color, font);
}
void Display::DrawRawString(const Point<int>& dst, const std::string& text, unsigned int color, int font) const
{
	DxLib::DrawStringToHandle(pos.x + dst.x, pos.y + dst.y, text.c_str(), color, font);
}
void Display::DrawRawString(int x,int y, const std::string& text, unsigned int color, int font) const
{
	DxLib::DrawStringToHandle(pos.x + x, pos.y + y, text.c_str(), color, font);
}



