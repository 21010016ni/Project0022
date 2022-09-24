#include "Config.hpp"

LONGLONG Application::WaitValue = 1000000 / 60;
LONGLONG Application::Count;

void callQuit(char* nextappPath, char* nextexename)
{
	SHELLEXECUTEINFO* m_appInfo;
	m_appInfo = new SHELLEXECUTEINFO;
	memset(m_appInfo, 0, sizeof(SHELLEXECUTEINFO));
	m_appInfo->cbSize = sizeof(SHELLEXECUTEINFO);
	m_appInfo->fMask = SEE_MASK_NO_CONSOLE;
	m_appInfo->hwnd = GetMainWindowHandle();
	m_appInfo->lpParameters = NULL;
	m_appInfo->lpDirectory = _T(nextappPath); //ƒpƒX
	m_appInfo->nShow = SW_SHOWNORMAL;
	m_appInfo->lpFile = _T(nextexename);
}

#ifdef DEBUG_CONSOLE
int main() { return Application::Main(GetModuleHandle(0), NULL, NULL, SW_SHOWDEFAULT); }
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR CmdLine, int iCmdShow) { return Application::Main(hInstance, hPreInst, CmdLine, iCmdShow); }
#endif // DEBUG_CONSOLE

