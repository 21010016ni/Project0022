#pragma once
#pragma warning(disable:28251)

#include <DxLib.h>
#include "Point.hpp"

// コメントアウトでウィンドウのみの実行
#define DEBUG_CONSOLE

class Application
{
	static LONGLONG WaitValue;
	static LONGLONG Count;

public:
	static Point<int> WindowSize;
	static int WindowColorBit;

	static const char8_t* Title;
	static const float Ver;

	static int WINAPI Main(HINSTANCE, HINSTANCE, LPSTR, int);
};

void callQuit(char* nextappPath, char* nextexename);
#ifdef DEBUG_CONSOLE
#pragma comment(linker, "/SUBSYSTEM:CONSOLE" )
int main();
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS" )
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR CmdLine, int iCmdShow);
#endif // DEBUG_CONSOLE

