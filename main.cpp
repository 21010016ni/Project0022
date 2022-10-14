#include "Config.hpp"
#include <random>
#include <chrono>
#include "Input.hpp"
#include "Manager.hpp"

Point<int> Application::WindowSize = {600,1024};
int Application::WindowColorBit = 32;
const char8_t* Application::Title = u8"Chromatic Cult";
const float Application::Ver = 0.00f;

std::mt19937 engine(std::random_device{}());

int Application::Main(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// 初期化
	SetMainWindowText("Now Loading...");
	SetGraphMode(WindowSize.x, WindowSize.y, WindowColorBit);
	// DxLib初期化ブロック
	//----------------------------------------------------------------<<
	ChangeWindowMode(TRUE);
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	//---------------------------------------------------------------->>
	// 初期化
	if(DxLib_Init() == -1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	// 初期化ブロック
	//----------------------------------------------------------------<<
	Manager::preset();

	//---------------------------------------------------------------->>
	// タイトル表示
	SetMainWindowText(std::format("{}  α{:4.2f}", (const char*)Title, Ver).c_str());
	while(!ProcessMessage())
	{
		// FPS制御.現在カウント取得
		Count = GetNowHiPerformanceCount();
		// キーボード入力状況更新
		Keyboard::update();
		Mouse::update();
		// 処理ブロック
		//----------------------------------------------------------------<<
		Manager::update();

		// スクリーンショット
		if(Keyboard::push(KEY_INPUT_F7))
			SaveDrawScreen(0, 0, WindowSize.x, WindowSize.y, std::format("ScreenShot_{:%m%d%H%M%OS}.jpg", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()}).c_str(), DX_IMAGESAVETYPE_JPEG);
		if(Keyboard::push(KEY_INPUT_F9))
			break;

		//---------------------------------------------------------------->>
		// 画面消去
		ClearDrawScreen();
		clsDx();
		// 描画ブロック
		//----------------------------------------------------------------<<
		Manager::draw();
		DrawString(0, 0, "abcABC123", 0xffffffff);
		DrawString(0, 30, (const char*)u8"🍻🍻🍻乾杯🍻🍻🍻", 0xffffffff);

		//---------------------------------------------------------------->>
		// 画面表示
		ScreenFlip();
		// FPS制御.ウェイト追加
		while(GetNowHiPerformanceCount() - Count < WaitValue);
	}
	// 終了処理
	SetMainWindowText("Process Quitting...");
	DxLib_End();
	return 0;
}

