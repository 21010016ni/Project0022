#pragma once
#include <string>
#include <deque>
#include <map>
#include <unordered_map>
#include "Display.hpp"

class Log
{
	struct Text
	{
		unsigned int tag;
		std::string value;
		Text(unsigned int tag, const char* data) :tag(tag), value(data) {}
	};
	static inline std::deque<Text> text;

public:
	enum Tag:unsigned int
	{
		battle,
		system,
		talk,
		debug,
	};

	static Display display;
	static inline size_t maxNum = 128, drawNum = 32;
	static inline std::map<unsigned int,bool> mute;

	static void push(unsigned int color, const char* data)
	{
		text.emplace_front(color, data);
		while(text.size() > maxNum)
			text.pop_back();
	}
	static void draw(size_t start);
};

