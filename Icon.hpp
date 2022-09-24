#pragma once
#include <DxLib.h>
#include "Point.hpp"

class Icon
{
	static inline int graph = -1;
	static inline unsigned char ln = 1;
	static inline int siz = 0;

public:
	static void load(const TCHAR* FileName, unsigned char line)
	{
		graph = LoadGraph(FileName);
		ln = line;
		GetGraphSize(graph, &siz, NULL);
		siz /= ln;
	}
	static void unload()
	{
		DeleteGraph(graph);
	}

	static void draw(const Point<int>& dst, int num)
	{
		DrawRectGraph(dst.x, dst.y, (num % ln) * siz, (num / ln) * siz, siz, siz, graph, TRUE);
	}
	static void draw(int x, int y, int num)
	{
		DrawRectGraph(x, y, (num % ln) * siz, (num / ln) * siz, siz, siz, graph, TRUE);
	}
	static int get_size()
	{
		return siz;
	}
};

