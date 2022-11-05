#pragma once
#include <string>
#include <stack>
#include <vector>

class BGM
{
	//struct Music
	//{
	//	int handle;
	//	std::string title;
	//};
	static inline std::vector<std::u8string> music;
	static inline std::vector<const std::u8string*> buf;
	static inline std::stack<const std::u8string*> stack;

public:
	enum Mode :unsigned char
	{
		roop = 1,
		shuffle = 2,
	};
	static inline std::u8string title;
	static inline unsigned char volume = 255;
	static inline unsigned char mode = 0;

	static void set(const char8_t* file);
	static void play(size_t id);
	static void play(const std::u8string& file);
	static void stop();
	static bool update();
	static void ChangeVolume(unsigned char v);
};

