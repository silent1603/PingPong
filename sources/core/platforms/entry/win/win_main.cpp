#include "Config_Platforms.h"

#if PLATFORMS_WINDOWS

const wchar_t g_appName[] = L"pingpong";
HWND g_hwnd;
HDC g_hdc;
HGLRC g_hglrc;

bool SetupPixelFormat(HDC);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow)
{
	return 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{

	}
	break;
	case WM_CLOSE:
	{

	}
	break
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	}
}
#endif