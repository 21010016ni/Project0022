#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Display.hpp"

class Menu
{
	class Item
	{
		int icon;	// アイコン番号
		std::string text;	// 選択肢テキスト
		std::function<void(int)> function;	// 選択時の処理

		std::vector<Item> child;	// 子選択肢

	public:
		int select;	// 選択中番号（-1で未選択）

		Item(int icon, const char* text) :icon(icon), text(text), select(-1)
		{

		}
		Item(int icon, const char* text, const std::function<void(int)>& function) :icon(icon), text(text), function(function), select(-1)
		{

		}

		const int Icon()const
		{
			return icon;
		}
		const char* Text()const
		{
			return text.c_str();
		}

		int size()const
		{
			return (int)child.size();
		}
		int empty()const
		{
			return child.empty();
		}
		void emplace_back(int icon, const char* text)
		{
			child.emplace_back(icon, text);
		}
		void emplace_back(int icon, const char* text, const std::function<void(int)>& function)
		{
			child.emplace_back(icon, text, function);
		}
		Item& back() { return child.back(); }
		void operator()(int v)const { if (function)function(v); }
		Item& operator[](int v) { return child.at(v); }
		operator bool()const { return select != -1; }
	};

	Point<int> itemSize;

	Display display;

public:
	Item root;

	Menu(const Point<int>& pos, const Point<int>& siz, const Point<int>& itemSize) :root(-1, ""), itemSize(itemSize), display(pos, siz)
	{
	}

	Menu(const Point<int>& pos, const Point<int>& siz, const Point<int>& itemSize, int font) :root(-1, ""), itemSize(itemSize), display(pos, siz, font)
	{
	}

	void SetFont(int font)
	{
		display.SetFont(font);
	}

	void select(const Point<int>& mouse);
	void draw(const Point<int>& mouse);
};

