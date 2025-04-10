#include "Globals.h"
#include "helpers/Macro_Helpers.h"
#if PLATFORMS_WINDOWS


HWND g_hwnd = 0;
HDC g_hdc = 0;
HGLRC g_hglrc = 0;
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

//GL setup
GLvoid render(GLvoid);

inline void fatal_error(char* msg)
{
	MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	//CreateWindow

	WNDCLASSEX wndClass = {};
	MSG msg;
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
		RECT rect = { 0,0,appProperties[(int)AppPropertiesDefine::WINDOW_WIDTH],appProperties[(int)AppPropertiesDefine::WINDOW_HEIGHT] };
		DWORD windowstyle = WS_OVERLAPPEDWINDOW;
		AdjustWindowRect(&rect, windowstyle, false);
		windowstyle |= WS_VISIBLE;
		g_hwnd = CreateWindowEx(0, wndClass.lpszClassName, TEXT(TO_STRING(APP_NAME)), windowstyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			0,
			0,
			hInstance,
			0);

		if (g_hwnd)
		{

			ShowWindow(g_hwnd, nCmdShow);
			UpdateWindow(g_hwnd);
			SET_FLAG(appProperties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
			while (HAS_FLAG(appProperties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING))
			{
				while (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
				{
					if (!GetMessage(&msg, NULL, 0, 0))
					{
						break;
					}
					if (msg.message == WM_QUIT)
					{
						UNSET_FLAG(appProperties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
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
		if (HAS_FLAG(appProperties[(int)AppPropertiesDefine::OPENGL_FEATURES], (int)AppOpenGLFeaturesDefine::ENABLE))
		{
			g_hdc = GetDC(hwnd);
			PIXELFORMATDESCRIPTOR pfd, * ppfd;
			int pixelformat;

			ppfd = &pfd;

			ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
			ppfd->nVersion = 1;
			ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
				PFD_DOUBLEBUFFER;
			ppfd->dwLayerMask = PFD_MAIN_PLANE;
			ppfd->iPixelType = PFD_TYPE_COLORINDEX;
			ppfd->cColorBits = 32;
			ppfd->cAlphaBits = 8;
			ppfd->cDepthBits = 24;
			ppfd->cAccumBits = 0;
			ppfd->cStencilBits = 8;

			pixelformat = ChoosePixelFormat(g_hdc, ppfd);

			if (pixelformat == 0)
			{
				MessageBox(NULL, TEXT("ChoosePixelFormat failed"), TEXT("Error"), MB_OK);
				PostQuitMessage(0);
			}

			if (SetPixelFormat(g_hdc, pixelformat, ppfd) == FALSE)
			{
				MessageBox(NULL, TEXT("SetPixelFormat failed"), TEXT("Error"), MB_OK);
				PostQuitMessage(0);
			}

			int major = 0;
			int minor = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);

			g_hglrc = wglCreateContext(g_hdc);
			if (HAS_FLAG(appProperties[(int)AppPropertiesDefine::OPENGL_FEATURES], (int)AppOpenGLFeaturesDefine::USE_EXTENSION))
			{
				wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
				wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

				int pixel_format_attribs[] = {
				WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
				WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
				WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB,         32,
				WGL_DEPTH_BITS_ARB,         24,
				WGL_STENCIL_BITS_ARB,       8,
				0
				};
				int pixel_format;
				UINT number_formats;
				wglChoosePixelFormatARB(g_hdc, pixel_format_attribs, 0, 1, &pixel_format, &number_formats);
				if (!number_formats) {
					OutputDebugString(TEXT("Failed to set the OpenGL 3.3 pixel format."));
				}
				PIXELFORMATDESCRIPTOR pfd;
				DescribePixelFormat(g_hdc, pixel_format, sizeof(pfd), &pfd);
				if (!SetPixelFormat(g_hdc, pixel_format, &pfd)) {
					fatal_error("Failed to set the OpenGL 3.3 pixel format.");
				}

				// Specify that we want to create an OpenGL 3.3 core profile context
				int gl33_attribs[] = {
					WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
					WGL_CONTEXT_MINOR_VERSION_ARB, 3,
					WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					0,
				};

				HGLRC gl33_context = wglCreateContextAttribsARB(g_hdc, 0, gl33_attribs);
				if (!gl33_context) {
					fatal_error("Failed to create OpenGL 3.3 context.");
				}

				if (!wglMakeCurrent(g_hdc, gl33_context)) {
					fatal_error("Failed to activate OpenGL 3.3 rendering context.");
				}
			}
			wglMakeCurrent(g_hdc, g_hglrc);

			GetClientRect(hwnd, &rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			glViewport(0, 0, width, height);
		}
	}
	break;
	case WM_SIZE:
	{
		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		
		if (HAS_FLAG(appProperties[(int)AppPropertiesDefine::OPENGL_FEATURES], (int)AppOpenGLFeaturesDefine::ENABLE))
		{
			glViewport(0, 0, width, height);
		}
	}
	break;
	case WM_CLOSE:
	{
		if (HAS_FLAG(appProperties[(int)AppPropertiesDefine::OPENGL_FEATURES], (int)AppOpenGLFeaturesDefine::ENABLE))
		{
			if (g_hglrc)
				wglDeleteContext(g_hglrc);

		}
		if (g_hdc)
			ReleaseDC(hwnd, g_hdc);
		g_hglrc = 0;
		g_hdc = 0;

		DestroyWindow(hwnd);
		UNSET_FLAG(appProperties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
	}
	break;

	break;
	case WM_DESTROY:
	{
		if (HAS_FLAG(appProperties[(int)AppPropertiesDefine::OPENGL_FEATURES], (int)AppOpenGLFeaturesDefine::ENABLE))
		{
			if (g_hglrc)
				wglDeleteContext(g_hglrc);
		}
		if (g_hdc)
			ReleaseDC(hwnd, g_hdc);
		PostQuitMessage(0);
		UNSET_FLAG(appProperties[(int)AppPropertiesDefine::FEATURES], (int)AppFeaturesDefine::IS_RUNNING);
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


GLvoid render(GLvoid)
{
	if (HAS_FLAG(appProperties[(int)AppPropertiesDefine::OPENGL_FEATURES], (int)AppOpenGLFeaturesDefine::ENABLE))
	{
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SwapBuffers(g_hdc);
	}
}

#endif