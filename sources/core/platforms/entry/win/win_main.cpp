#include "Config_Platforms.h"

#if PLATFORMS_WINDOWS

const wchar_t g_appName[] = L"pingpong";
HWND g_hwnd = 0;
HDC g_hdc = 0;
HGLRC g_hglrc = 0;
bool g_isRunning = false;
#define WIDTH           300 
#define HEIGHT          200 

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL setupPixelFormat(HDC);

//GL setup
GLvoid render(GLvoid);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndClass = {};

	wndClass.style = 0;
	wndClass.lpfnWndProc = (WNDPROC)MainWndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance,g_appName);
	wndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = g_appName;
	wndClass.lpszClassName = g_appName;

	if (RegisterClass(&wndClass))
	{
		g_hwnd = CreateWindowEx(0, wndClass.lpszClassName,L"Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			hInstance,
			0);

		if (g_hwnd)
		{

				ShowWindow(g_hwnd, nCmdShow);
				UpdateWindow(g_hwnd);
				g_isRunning = true;
				while (g_isRunning)
				{
					while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
					{
						if (msg.message == WM_QUIT)
						{
							g_isRunning = false;
						}
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					render();
				}
		}
	}

	return 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	LONG    lRet = 0;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		g_hdc = GetDC(hwnd);
		if (!setupPixelFormat(g_hdc))
			PostQuitMessage(0);

		g_hglrc = wglCreateContext(g_hdc);
		wglMakeCurrent(g_hdc, g_hglrc);
		GetClientRect(hwnd, &rect);
	}
		break;
	case WM_SIZE:
	{
		GetClientRect(hwnd, &rect);
	}
	break;
	case WM_CLOSE:
	{
		if (g_hglrc)
			wglDeleteContext(g_hglrc);
		if (g_hdc)
			ReleaseDC(hwnd, g_hdc);
		g_hglrc = 0;
		g_hdc = 0;

		DestroyWindow(hwnd);
		g_isRunning = false;
	}
	break;

	break;
	case WM_DESTROY:
	{
		if (g_hglrc)
			wglDeleteContext(g_hglrc);
		if (g_hdc)
			ReleaseDC(hwnd, g_hdc);
		PostQuitMessage(0);
		g_isRunning = false;
	}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		}
		break;
	case WM_PAINT:
	default:
		lRet = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return lRet;
}


BOOL setupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd, * ppfd;
	int pixelformat;

	ppfd = &pfd;

	ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	ppfd->nVersion = 1;
	ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	ppfd->dwLayerMask = PFD_MAIN_PLANE;
	ppfd->iPixelType = PFD_TYPE_COLORINDEX;
	ppfd->cColorBits = 8;
	ppfd->cDepthBits = 16;
	ppfd->cAccumBits = 0;
	ppfd->cStencilBits = 0;

	pixelformat = ChoosePixelFormat(hdc, ppfd);

	if (pixelformat == 0)
	{
		MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
	{
		MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

GLvoid render(GLvoid)
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SwapBuffers(g_hdc);
}

#endif