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
		std::function<void(int, Item&)> function;	// �I�����̏���

		std::vector<Item> child;	// �q�I����

	public:
		int icon;	// �A�C�R���ԍ�
		std::string text;	// �I�����e�L�X�g
		int select;	// �I�𒆔ԍ��i-1�Ŗ��I���j

		Item(int icon, const char* text) :icon(icon), text(text), select(-1)
		{

		}
		Item(int icon, const char* text, const std::function<void(int, Item&)>& function) :icon(icon), text(text), function(function), select(-1)
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
		void emplace_back(int icon, const char* text)
		{
			child.emplace_back(icon, text);
		}
		void emplace_back(int icon, const char* text, const std::function<void(int, Item&)>& function)
		{
			child.emplace_back(icon, text, function);
		}
		Item& back() { return child.back(); }
		void operator()(int v, Item& i)const { if (function)function(v, i); }
		Item& operator[](int v) { return child.at(v); }
		operator bool()const { return select != -1; }
	};

private:
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
	bool hit(const Point<int>& t)const
	{
		return display.hit(t);
	}

	void SetFont(int font)
	{
		display.SetFont(font);
	}

	void select(const Point<int>& mouse);
	void draw(const Point<int>& mouse);
};

