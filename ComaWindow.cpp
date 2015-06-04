#include "stdafx.h"
#include "ComaWindow.h"
#include <sstream>
#include "WindowEvent.h"
using namespace Coma2D;

ComaWindow* comaWindow;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return comaWindow->messageProc(hWnd, uMsg, wParam, lParam);
}

ComaWindow::ComaWindow(HINSTANCE hInstance)
	:hInstance(0), hWnd(0), activated(false), minimized(false), maximized(false), resizing(false), running(false), fullscreen(false), created(false)
{
	this->hInstance = hInstance;
	comaWindow = this;

	setWindowPosition(CW_USEDEFAULT, CW_USEDEFAULT);
	setScreenSize(800, 480);
	setMaxScreenSize(-1, -1);
	setMinScreenSize(-1, -1);

	hIcon = LoadIcon(0, IDI_APPLICATION);
	hCursor = LoadCursor(0, IDC_ARROW);
	dwStyle = WS_OVERLAPPEDWINDOW;
	dwStyleEx = 0;
	windowTitle = "Coma2DWindow";
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
	windowClass.hIcon = hIcon;
	windowClass.hCursor = hCursor;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;

	if (!RegisterClass(&windowClass))
		return false;

	RECT rect = screenRect;
	AdjustWindowRectEx(&rect, dwStyle, false, dwStyleEx);

	hWnd = CreateWindowEx(
		dwStyleEx,
		className,
		windowTitle,
		dwStyle,
		windowPosition.x, windowPosition.y, 
		rect.right - rect.left, rect.bottom - rect.top,
		0, 0, hInstance, 0);
	
	if (!hWnd)
		return false;
	
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);


	//SetWindowStatus
	GetWindowRect(hWnd, &windowRect);
	windowPosition = { windowRect.left, windowRect.top };

	if (fullscreen)
	{
		fullscreen = false;
		setFullScreen(true, fullscreenSize.x, fullscreenSize.y);
	}
	else if (minimized)
		PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE, 0);
	else if (maximized)
		PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_MAXIMIZE, 0);
	
	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
	created = true;
	dispatchEvent(new WindowEvent(WindowEvent::CREATED, this));

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
			dispatchEvent(new WindowEvent(WindowEvent::UPDATE, this));
			Sleep(5);
		}
	}
	return true;
}
void ComaWindow::close()
{
	if (hWnd)
		PostMessage(hWnd, WM_CLOSE, 0, 0);
}

LRESULT ComaWindow::messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			activated = false;
			if (fullscreen)
			{
				ChangeDisplaySettings(NULL, 0);
				minimizeWindow();
			}
			dispatchEvent(new WindowEvent(WindowEvent::ACTIVATED, this));
		}
		else
		{
			activated = true;
			if (fullscreen)
			{
				setFullScreen(true, fullscreenSize.x, fullscreenSize.y);
			}
			dispatchEvent(new WindowEvent(WindowEvent::INACTIVATED, this));
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		resizing = true;
		dispatchEvent(new WindowEvent(WindowEvent::ENTER_RESIZE, this));
		return 0;
	case WM_EXITSIZEMOVE:
		resizing = false;
		dispatchEvent(new WindowEvent(WindowEvent::EXIT_RESIZE, this));
		return 0;
	case WM_SIZE:
		updateRectData();
		if (wParam == SIZE_MAXIMIZED)
		{
			minimized = false;
			maximized = true;
			dispatchEvent(new WindowEvent(WindowEvent::MAXIMIZED, this));
		}
		else if (wParam == SIZE_MINIMIZED)
		{
			minimized = true;
			maximized = false;
			dispatchEvent(new WindowEvent(WindowEvent::MINIMIZED, this));
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (maximized)
			{
				maximized = false;
				minimized = false;
				dispatchEvent(new WindowEvent(WindowEvent::RESTORED, this));
			}
			else if (minimized)
			{
				maximized = false;
				minimized = false;
				changeWindowSize(windowPosition, screenRect);
				dispatchEvent(new WindowEvent(WindowEvent::RESTORED, this));
			}
			else if (resizing)
			{
				dispatchEvent(new WindowEvent(WindowEvent::RESIZING, this));
			}
		}
		dispatchEvent(new WindowEvent(WindowEvent::RESIZE, this));
		return 0;
	case WM_MOVE:
		if (resizing)
			updateRectData();
		dispatchEvent(new WindowEvent(WindowEvent::MOVING, this));
		return 0;
	case WM_DESTROY:
		created = false;
		dispatchEvent(new WindowEvent(WindowEvent::DESTROYED, this));
		PostQuitMessage(0);
		return 0;
	case WM_GETMINMAXINFO:
		if (maxScreenRect.right - maxScreenRect.left > 0)
			((MINMAXINFO*)lParam)->ptMaxTrackSize.x = maxScreenRect.right - maxScreenRect.left;
		if (maxScreenRect.bottom - maxScreenRect.top > 0)
			((MINMAXINFO*)lParam)->ptMaxTrackSize.y = maxScreenRect.bottom - maxScreenRect.top;
		if (minScreenRect.right - minScreenRect.left > 0)
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = minScreenRect.right - minScreenRect.left;
		if (minScreenRect.bottom - minScreenRect.top > 0)
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = minScreenRect.bottom - minScreenRect.top;
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

bool ComaWindow::setMaxScreenSize(RECT size)
{
	maxScreenRect = size;
	return true;
}
bool ComaWindow::setMaxScreenSize(int width, int height)
{
	return setMaxScreenSize({ 0, 0, width, height });
}
bool ComaWindow::setMinScreenSize(RECT size)
{
	minScreenRect = size;
	return true;
}
bool ComaWindow::setMinScreenSize(int width, int height)
{
	return setMinScreenSize({ 0, 0, width, height });
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
	if (fullscreen)
		return{ 0, 0, fullscreenSize.x, fullscreenSize.y };
	updateRectData();
	return windowRect;
}
POINT ComaWindow::getWindowPosition()
{
	if (fullscreen)
		return POINT{ 0, 0 };
	updateRectData();
	return windowPosition;
}
RECT ComaWindow::getScreenSize()
{
	if (fullscreen)
		return RECT{ 0, 0, fullscreenSize.x, fullscreenSize.y };
	updateRectData();
	return screenRect;
}



//Window Size Function
bool ComaWindow::changeWindowSize(POINT position, RECT screenSize)
{
	if (!hWnd)
		return false;
	if (fullscreen)
		return false;

	RECT rect = screenSize;
	AdjustWindowRectEx(&rect, dwStyle, false, dwStyleEx);

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
	if (fullscreen)
		return false;

	if (isMinimized())
		return false;
	if (!GetWindowRect(hWnd, &windowRect))
		return false;
	if (!GetClientRect(hWnd, &screenRect))
		return false;
	windowPosition = { windowRect.left, windowRect.top };
	
	return true;
}

void ComaWindow::minimizeWindow()
{
	if (hWnd) 
		PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE, 0);
	else
	{
		minimized = true;
		maximized = false;
	}
}
void ComaWindow::maximizeWindow()
{
	if (hWnd)
		PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_MAXIMIZE, 0);
	else
	{
		minimized = false;
		maximized = true;
	}
}
void ComaWindow::restoreWindow()
{
	if (hWnd)
		PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
	else
	{
		minimized = false;
		maximized = false;
	}
}
bool ComaWindow::setFullScreen(bool mode, int width, int height)
{
	if (mode)
		fullscreenSize = { width, height };
	if (!hWnd)
	{
		fullscreen = mode;
		return true;
	}
	
	if (mode)
	{
		fullscreen = true;
		LONG style = GetWindowLong(hWnd, GWL_STYLE);
		LONG styleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
		SetWindowLong(hWnd, GWL_STYLE, style&~WS_OVERLAPPEDWINDOW | WS_POPUP);
		SetWindowLong(hWnd, GWL_EXSTYLE, style | WS_EX_TOPMOST);
		DEVMODE dm;
		ZeroMemory(&dm, sizeof(DEVMODE));
		dm.dmSize = sizeof(DEVMODE);
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		dm.dmBitsPerPel = 32;
		dm.dmPelsWidth = width;
		dm.dmPelsHeight = height;
		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			ChangeDisplaySettings(&dm, 0);
			fullscreen = false;
			SetWindowLong(hWnd, GWL_STYLE, dwStyle);
			SetWindowLong(hWnd, GWL_EXSTYLE, dwStyleEx);
			changeWindowSize(windowPosition, screenRect);
			return false;
		}
		fullscreenSize = { width, height };
		SetWindowPos(hWnd, NULL, 0, 0, width, height, 0);
		dispatchEvent(new WindowEvent(WindowEvent::ENTER_FULLSCREEN,this));
		return true;
	}
	else if (!mode)
	{
		ChangeDisplaySettings(NULL, 0);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle);
		SetWindowLong(hWnd, GWL_EXSTYLE, dwStyleEx);
		fullscreen = false;

		changeWindowSize(windowPosition, screenRect);
		dispatchEvent(new WindowEvent(WindowEvent::EXIT_FULLSCREEN, this));
		return true;
	}
	return false;
}

//Window Status Setter
bool ComaWindow::setIcon(HICON hIcon)
{
	if (hWnd)
		return false;
	this->hIcon = hIcon;
	return true;
}
bool ComaWindow::setCursor(HCURSOR hCursor)
{
	if (hWnd)
		return false;
	this->hCursor = hCursor;
	return true;
}
bool ComaWindow::setStyle(DWORD dwStyle)
{
	if (!hWnd)
	{
		this->dwStyle |= dwStyle;
		return true;
	}
	if (!fullscreen)
	{
		this->dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		this->dwStyle |= dwStyle;
		SetWindowLong(hWnd, GWL_STYLE, this->dwStyle);
		return true;
	}
	return false;
}
bool ComaWindow::setStyleEx(DWORD dwStyleEx)
{
	if (!hWnd)
	{
		this->dwStyleEx |= dwStyleEx;
		return true;
	}
	if (!fullscreen)
	{
		this->dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
		this->dwStyleEx |= dwStyleEx;
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, this->dwStyleEx);
		return true;
	}
	return false;
}
bool ComaWindow::removeStyle(DWORD dwStyle)
{
	if (!hWnd)
	{
		this->dwStyle &= ~dwStyle;
		return true;
	}
	if (!fullscreen)
	{
		this->dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		this->dwStyle &= ~dwStyle;
		SetWindowLong(hWnd, GWL_STYLE, this->dwStyle);
		return true;
	}
	return false;
}
bool ComaWindow::removeStyleEx(DWORD dwStyleEx)
{
	if (!hWnd)
	{
		this->dwStyleEx &= ~dwStyleEx;
		return true;
	}
	if (!fullscreen)
	{
		this->dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
		this->dwStyleEx &= ~dwStyleEx;
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, this->dwStyleEx);
		return true;
	}
	return false;
}
bool ComaWindow::setTitle(const char* name)
{
	windowTitle = name;
	if (!hWnd)
		return true;
	if (SetWindowText(hWnd, windowTitle))
		return true;
	return false;
}
bool ComaWindow::setTitle(std::string name)
{
	return setTitle(name.c_str());
}