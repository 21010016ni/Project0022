#pragma once
#include <memory>
#include <string>
#include <deque>
#include <map>
#include "Display.hpp"

#pragma warning(disable:26812)

unsigned int color(const std::u8string& u8);
std::u8string color(unsigned int value);

class Log
{
public:
	struct Text
	{
		std::shared_ptr<std::pair<unsigned int,std::u8string>> value;
		Text() :value() {}
		Text(unsigned int tag, const char8_t* data) :value(new std::pair<unsigned int, std::u8string>{ tag,std::u8string(data) }) {}
		Text(unsigned int tag, const std::u8string& data) :value(new std::pair<unsigned int, std::u8string>{ tag,data }) {}
		operator bool()const { return (bool)value; }
		const unsigned int& tag()const { return value->first; }
		const std::u8string& text()const { return value->second; }
		bool HasSpeaker()const;
	};

	static Display display;
private:
	static inline std::deque<Text> text;

public:
	enum Tag :unsigned int
	{
		battle,
		system,
		talk,
		debug,
		none,
	};

	static int maxNum, drawNum;
	static inline std::map<unsigned int, bool> mute;

	Log() = delete;

	static bool hit(const Point<int>& t)
	{
		return display.hit(t);
	}
	static void push(unsigned int tag, const char8_t* data)
	{
		text.emplace_front(tag, data);
		while (text.size() > maxNum)
			text.pop_back();
	}
	static void push(Text& data)
	{
		text.emplace_front(data);
		while (text.size() > maxNum)
			text.pop_back();
	}
	static const Text& latest(Tag type)
	{
		static auto none = Text();
		for (const auto& i : text)
		{
			if (i.tag() == type)
			{
				return i;
			}
		}
		return none;
	}
	static void draw(size_t start);
};

