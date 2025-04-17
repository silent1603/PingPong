#include "GlobalsData.h"

#if PLATFORMS_WINDOWS
#include "helpers/Macro_Helpers.h"
#include "Input/InputSystem.h"
#include "logger/ErrorLogger.h"

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


inline void input()
{
	if(IsKeyDown(VK_ESCAPE))
	{
		UNSET_FLAG(appInformation.properties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
	}
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	// CreateWindow
	appInformation.hInstance = hInstance;
	WNDCLASSEX wndClass = {};
	MSG msg;
	DEVMODE dmScreenSettings;
	int maxScreenWidth;
	int maxScreenHeight;
	int posX, posY;
	{
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndClass.lpfnWndProc = (WNDPROC)MainWndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInstance;
		wndClass.hIcon = LoadIcon(hInstance, TEXT(TO_STRING(APP_NAME)));
		wndClass.hIconSm = wndClass.hIcon;
		wndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = TEXT(TO_STRING(APP_NAME));
		wndClass.lpszClassName = TEXT(TO_STRING(APP_NAME));
		wndClass.cbSize = sizeof(WNDCLASSEX);
	}

	if (RegisterClassEx(&wndClass))
	{
		maxScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		maxScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		appInformation.properties[(int)AppPropertiesDefine::WINDOW_WIDTH] = maxScreenWidth;
		appInformation.properties[(int)AppPropertiesDefine::WINDOW_HEIGHT] = maxScreenWidth;
		if (HAS_FLAG(appInformation.properties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::FULLSCREEN))
		{
			// If full screen set the screen to maximum size of the users desktop and 32bit.
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)maxScreenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)maxScreenHeight;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Change the display settings to full screen.
			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			// Set the position of the window to the top left corner.
			posX = posY = 0;
		} 
		else
		{
			// Place the window in the middle of the screen.
			posX = (GetSystemMetrics(SM_CXSCREEN) -  appInformation.properties[(int)AppPropertiesDefine::WINDOW_WIDTH])  / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) -  appInformation.properties[(int)AppPropertiesDefine::WINDOW_HEIGHT]) / 2;
		}
		DWORD windowstyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_VISIBLE;

		appInformation.hwnd = CreateWindowEx(0, wndClass.lpszClassName, TEXT(TO_STRING(APP_NAME)), windowstyle,
								posX,
								posY,
								appInformation.properties[(int)AppPropertiesDefine::WINDOW_WIDTH],
								appInformation.properties[(int)AppPropertiesDefine::WINDOW_HEIGHT],
								0,
								0,
								hInstance,
								0);

		if (appInformation.hwnd)
		{

			ShowWindow(appInformation.hwnd, nCmdShow);
			SetForegroundWindow(appInformation.hwnd);
			SetFocus(appInformation.hwnd);
			ShowCursor(false);
			UpdateWindow(appInformation.hwnd);
			SET_FLAG(appInformation.properties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
			while (HAS_FLAG(appInformation.properties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING))
			{

				while (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
				{
					if (!GetMessage(&msg, NULL, 0, 0))
					{
						break;
					}
					if (msg.message == WM_QUIT)
					{
						UNSET_FLAG(appInformation.properties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
					}
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				input();


			}
		}
	}

	return 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	LONG lRet = 0;
	switch (uMsg)
	{
	case WM_CREATE:
	{

		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
	
	}
	break;
	case WM_SIZE:
	{
		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
	}
	break;
	case WM_DESTROY:
	case WM_CLOSE:
	{

		if (appInformation.hdc)
		{
			ReleaseDC(hwnd, appInformation.hdc);
		}

		appInformation.hdc = 0;

		DestroyWindow(hwnd);

		ShowCursor(true);

		if (HAS_FLAG(appInformation.properties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::FULLSCREEN))
		{
			ChangeDisplaySettings(NULL, 0);
		}

		appInformation.hwnd = NULL;
		UnregisterClass(TEXT(TO_STRING(APP_NAME)),appInformation.hInstance);
		appInformation.hInstance = NULL;

		UNSET_FLAG(appInformation.properties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
		PostQuitMessage(0);
	}
	break;
	case WM_KEYDOWN:
	{
		KeyDown((unsigned int)wParam);
	}
	break;
	case WM_KEYUP:
	{
		KeyUp((unsigned int)wParam);
	}
	break;
	case WM_PAINT:
	default:
		lRet = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return lRet;
}


#endif