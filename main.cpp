#include "Config.hpp"
#include <random>
#include <chrono>
#include "Input.hpp"
#include "Manager.hpp"

Point<int> Application::WindowSize = { 600,1024 };
int Application::WindowColorBit = 32;
const char* Application::Title = "Chromatic Cult";
const float Application::Ver = 0.00f;

std::mt19937 engine(std::random_device{}());

int Application::Main(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ������
	SetMainWindowText("Now Loading...");
	SetGraphMode(WindowSize.x, WindowSize.y, WindowColorBit);
	// DxLib�������u���b�N
	//----------------------------------------------------------------<<
	ChangeWindowMode(TRUE);

	//---------------------------------------------------------------->>
	// ������
	if (DxLib_Init() == -1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	// �������u���b�N
	//----------------------------------------------------------------<<
	Manager::preset();

	//---------------------------------------------------------------->>
	// �^�C�g���\��
	SetMainWindowText(std::format("{}  ��{:4.2f}", Title, Ver).c_str());
	while (!ProcessMessage())
	{
		// FPS����.���݃J�E���g�擾
		Count = GetNowHiPerformanceCount();
		// �L�[�{�[�h���͏󋵍X�V
		Keyboard::update();
		Mouse::update();
		// �����u���b�N
		//----------------------------------------------------------------<<
		Manager::update();

		// �X�N���[���V���b�g
		if (Keyboard::push(KEY_INPUT_F7))
			SaveDrawScreen(0, 0, WindowSize.x, WindowSize.y, std::format("ScreenShot_{:%m%d%H%M%OS}.jpg", std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }).c_str(), DX_IMAGESAVETYPE_JPEG);
		if (Keyboard::push(KEY_INPUT_F9))
			break;

		//---------------------------------------------------------------->>
		// ��ʏ���
		ClearDrawScreen();
		clsDx();
		// �`��u���b�N
		//----------------------------------------------------------------<<
		Manager::draw();

		//---------------------------------------------------------------->>
		// ��ʕ\��
		ScreenFlip();
		// FPS����.�E�F�C�g�ǉ�
		while (GetNowHiPerformanceCount() - Count < WaitValue);
	}
	// �I������
	SetMainWindowText("Process Quitting...");
	DxLib_End();
	return 0;
}

