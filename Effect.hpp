#pragma once
#include <string>
#include <vector>
#include <list>
#include "Point.hpp"

class Effect
{
public:
	enum class Pos :char
	{
		leftup,
		rightup,
		leftdown,
		rightdown,
		center
	};

private:
	struct Data
	{
		int graph;
		Point<int> size;
		Point<int> num;
		int wait;
		int se;
		mutable unsigned char volume;

		Data(int graph, int wn, int hn, int wait, int se, unsigned char volume);

		void draw(int x, int y, int t)const;
		void play()const;
	};
	struct Inst
	{
		const Data* data;
		Point<int> dst;
		int time;
		Pos pos;

		Inst(const Data* data, int x, int y, Pos pos);

		void play()const;
	};

	static inline std::vector<Data> data;
	static inline std::list<Inst> list;

public:
	static inline unsigned char volume = 255;

	static void load(const std::u8string& FileName);
	static void load(int graph, int wn, int hn, int wait, int se);

	static void set(int id, int x, int y, Pos pos);
	static void play();
};

