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
	static inline std::vector<std::string> music;
	static inline std::stack<const std::string*> stack;

public:
	static inline std::string title;
	static inline unsigned char volume = 255;

	static void set(const char* file);
	static bool update();
	static void ChangeVolume(unsigned char v);
};

