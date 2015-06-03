#include "stdafx.h"
#include "ComaWindow.h"
#include <sstream>
using namespace Coma2D;

ComaWindow* comaWindow;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return comaWindow->messageProc(hWnd, uMsg, wParam, lParam);
}

ComaWindow::ComaWindow(HINSTANCE hInstance)
	:hInstance(0), hWnd(0), activated(false), minimized(false), maximized(false), resizing(false), running(false)
{
	this->hInstance = hInstance;
	comaWindow = this;

	setWindowPosition(CW_USEDEFAULT, CW_USEDEFAULT);
	setScreenSize(800, 480);
}


ComaWindow::~ComaWindow()
{
}


bool ComaWindow::createWindow()
{
	if (hWnd)
		return false;
	const char* className = "Coma2DWindow";
	WNDCLASS windowClass;
	
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;

	if (!RegisterClass(&windowClass))
		return false;

	RECT rect = screenRect;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	
	hWnd = CreateWindow(
		className,
		"Coma2DWindow",
		WS_OVERLAPPEDWINDOW,
		windowPosition.x, windowPosition.y, 
		rect.right - rect.left, rect.bottom - rect.top,
		0, 0, hInstance, 0);
	
	if (!hWnd)
		return false;
	
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	GetWindowRect(hWnd, &windowRect);
	windowPosition = { windowRect.left, windowRect.top };
	
	return true;
}

bool ComaWindow::run()
{
	if (!hWnd)
		return false;
	running = true;
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//=============For Debug================
			std::ostringstream outs;
			outs.precision(6);
			outs << "X: " << windowPosition.x << "  Y: " << windowPosition.y << "  Width: " << screenRect.right << "  Height: " << screenRect.bottom << "   Resizing: " << isResizing();
			//======================================

			SetWindowText(hWnd, outs.str().c_str());
			Sleep(5);
		}
	}
	return true;
}

LRESULT ComaWindow::messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
			activated = false;
		else
			activated = true;
		return 0;
	case WM_ENTERSIZEMOVE:
		resizing = true;
		return 0;
	case WM_EXITSIZEMOVE:
		resizing = false;
		return 0;
	case WM_SIZE:
		if (wParam == SIZE_MAXIMIZED)
		{
			minimized = false;
			maximized = true;
		}
		else if (wParam == SIZE_MINIMIZED)
		{
			minimized = true;
			maximized = false;
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (maximized)
			{
				maximized = false;
				minimized = false;
			}
			else if (minimized)
			{
				maximized = false;
				minimized = false;
				changeWindowSize(windowPosition, screenRect);
			}
			else if (resizing)
			{

			}
		}
		updateRectData();
		return 0;
	case WM_MOVE:
		if (resizing)
			updateRectData();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}





//Window Size Setter
bool ComaWindow::setWindowPosition(POINT position)
{
	if (!hWnd)
	{
		windowPosition = position;
		return true;
	}
	return changeWindowSize(position, screenRect);
}
bool ComaWindow::setWindowPosition(int x, int y)
{
	return setWindowPosition(POINT{ x, y });
}

bool ComaWindow::setScreenSize(RECT size)
{
	if (!hWnd)
	{
		screenRect = size;
		return true;
	}
	return changeWindowSize(windowPosition, size);
}
bool ComaWindow::setScreenSize(int width, int height)
{
	return setScreenSize(RECT{ 0, 0, width, height });
}

bool ComaWindow::setWindowRect(RECT rect)
{
	RECT size = { 0, 0, rect.right, rect.bottom };
	POINT position = { rect.left, rect.top };
	if (!hWnd)
	{
		screenRect = size;
		windowPosition = position;
		return true;
	}
	return changeWindowSize(position, size);
}
bool ComaWindow::setWindowRect(int x, int y, int width, int height)
{
	return setWindowRect(RECT{ x, y, width, height });
}


//Window Size Getter
RECT ComaWindow::getWindowRect()
{
	updateRectData();
	return windowRect;
}
POINT ComaWindow::getWindowPosition()
{
	updateRectData();
	return windowPosition;
}
RECT ComaWindow::getScreenSize()
{
	return screenRect;
}



//Window Size Function
bool ComaWindow::changeWindowSize(POINT position, RECT screenSize)
{
	if (!hWnd)
		return false;

	RECT rect = screenSize;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	if (!SetWindowPos(
		hWnd, NULL, position.x, position.y,
		rect.right - rect.left, rect.bottom - rect.top,
		0))
		return false;
	
	updateRectData();
	return true;
}
bool ComaWindow::updateRectData()
{
	if (isMinimized())
		return false;
	if (!GetWindowRect(hWnd, &windowRect))
		return false;
	if (!GetClientRect(hWnd, &screenRect))
		return false;
	windowPosition = { windowRect.left, windowRect.top };

	return true;
}