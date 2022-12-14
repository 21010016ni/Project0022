#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Display.hpp"

class Menu
{
public:
	class Item
	{
		std::function<void(int, Item&)> function;	// 選択時の処理

		std::vector<Item> child;	// 子選択肢

	public:
		int icon;	// アイコン番号
		std::u8string text;	// 選択肢テキスト
		int select;	// 選択中番号（-1で未選択）

		Item(int icon, const char8_t* text) :icon(icon), text(text), select(-1)
		{

		}
		Item(int icon, const char8_t* text, const std::function<void(int, Item&)>& function) :icon(icon), text(text), function(function), select(-1)
		{

		}

		int size()const
		{
			return (int)child.size();
		}
		int empty()const
		{
			return child.empty();
		}
		void emplace_back(int icon, const char8_t* text)
		{
			child.emplace_back(icon, text);
		}
		void emplace_back(int icon, const char8_t* text, const std::function<void(int, Item&)>& function)
		{
			child.emplace_back(icon, text, function);
		}
		Item& back() { return child.back(); }
		void operator()(int v, Item& i)const { if (function)function(v, i); }
		Item& operator[](int v) { return child.at(v); }
		operator bool()const { return select != -1; }
	};

private:
	static Display display;

	static Point<int> itemSize;
	static inline int se[2] = { -1,-1 };
	static inline bool prevFlag = false;

public:
	static Item root;

	Menu() = delete;

	static bool hit(const Point<int>& t)
	{
		return display.hit(t);
	}
	static void SetSE(int select, int decision)
	{
		se[0] = select;
		se[1] = decision;
	}
	static void SetSEVolume(unsigned char v);

	static void select(const Point<int>& mouse);
	static void draw(const Point<int>& mouse);
};

