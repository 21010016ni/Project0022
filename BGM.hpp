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
	static inline std::stack<const std::u8string*> stack;

public:
	static inline std::u8string title;
	static inline unsigned char volume = 255;

	static void set(const char8_t* file);
	static bool update();
	static void ChangeVolume(unsigned char v);
};

