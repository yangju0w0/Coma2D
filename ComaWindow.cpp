#include "stdafx.h"
#include "ComaWindow.h"
using namespace coma2d;

ComaWindow* comaWindow;

// 메인 윈도우 프로시저
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return comaWindow->messageProc(hWnd, uMsg, wParam, lParam);
	//받은 메시지를 모두 messageProc 함수로 넘긴다. (클래스 내부에서 처리)
}


// 생성자
ComaWindow::ComaWindow(HINSTANCE hInstance, int nCmdShow)
	:hInstance(0), hWnd(0), created(false), running(false), activated(false), fullscreen(false),
	maximized(false), minimized(false), resizing(false)
{
	this->hInstance = hInstance;
	comaWindow = this;

	windowData.icon = LoadIcon(0, IDI_APPLICATION);
	windowData.cursor = LoadCursor(0, IDC_ARROW);
	windowData.backgroundColor = (HBRUSH)GetStockObject(NULL_BRUSH);
	windowData.style = WS_OVERLAPPEDWINDOW;
	windowData.styleEx = NULL;
	windowData.title = TEXT("ComaWindow");
	windowData.nCmdShow = nCmdShow;
	windowData.windowPosition = { CW_USEDEFAULT, CW_USEDEFAULT };
	windowData.windowSize = { 0, 0, CW_USEDEFAULT, CW_USEDEFAULT };
	windowData.minWindowSize = { 0, 0, -1, -1 };
	windowData.maxWindowSize = { 0, 0, -1, -1 };
}


// 소멸자
ComaWindow::~ComaWindow()
{
}

// 윈도우 생성
bool ComaWindow::createWindow()
{
	if (isCreated())
		return false;

	//윈도우 클래스 등록
	TCHAR* className = TEXT("ComaWindow");
	WNDCLASS windowClass;
	
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = windowData.icon;
	windowClass.hCursor = windowData.cursor;
	windowClass.hbrBackground = windowData.backgroundColor;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;

	if (!RegisterClass(&windowClass))
		return false;
	

	//윈도우 생성
	hWnd = CreateWindowEx(
		windowData.styleEx, className,
		windowData.title,
		windowData.style,
		windowData.windowPosition.x, windowData.windowPosition.y, //x,y
		windowData.windowSize.right - windowData.windowSize.left, //width
		windowData.windowSize.bottom - windowData.windowSize.top, //height
		0, 0, hInstance, 0);
	
	if (!hWnd)
		return false;

	created = true;

	if (isMaximized())
		windowData.nCmdShow = SW_SHOWMAXIMIZED;
	else if (isMinimized())
		windowData.nCmdShow = SW_SHOWMINIMIZED;

	//윈도우 출력
	ShowWindow(hWnd, windowData.nCmdShow);
	UpdateWindow(hWnd);

	if (isFullscreen())
	{

	}
	
	return true;
}

// 윈도우 루프
bool ComaWindow::run()
{
	if (!hWnd || isRunning())
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
			Sleep(1);
		}
	}
	return true;
}

// 메세지 처리부
LRESULT ComaWindow::messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			activated = false;
		}
		else if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
		{
			activated = true;
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		return 0;

	case WM_EXITSIZEMOVE:
		moving = false;
		resizing = false;
		return 0;

	case WM_MOVE:
		return 0;

	case WM_MOVING:
		moving = true;
		return 0;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			minimized = true;
			maximized = false;
			resizing = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			minimized = false;
			maximized = true;
			resizing = false;
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (maximized)
			{
				maximized = false;
				minimized = false;
				resizing = false;
			}
			else if (minimized)
			{
				maximized = false;
				minimized = false;
				resizing = false;
			}
			else if (resizing)
			{
				
			}
		}
		return 0;

	case WM_SIZING:
		resizing = true;
		return 0;

	case WM_GETMINMAXINFO:
		if (windowData.maxWindowSize.right - windowData.maxWindowSize.left > 0)
			((MINMAXINFO*)lParam)->ptMaxTrackSize.x = windowData.maxWindowSize.right - windowData.maxWindowSize.left;
		if (windowData.maxWindowSize.bottom - windowData.maxWindowSize.top > 0)
			((MINMAXINFO*)lParam)->ptMaxTrackSize.y = windowData.maxWindowSize.bottom - windowData.maxWindowSize.top;
		if (windowData.minWindowSize.right - windowData.minWindowSize.left > 0)
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = windowData.minWindowSize.right - windowData.minWindowSize.left;
		if (windowData.minWindowSize.bottom - windowData.minWindowSize.top > 0)
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = windowData.minWindowSize.bottom - windowData.minWindowSize.top;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		created = false;
		running = false;
		activated = false;
		fullscreen = false;
		maximized = false;
		minimized = false;
		resizing = false;
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//Setter
bool ComaWindow::setIcon(HICON icon)
{
	if (isCreated())
		return false;
	windowData.icon = icon;
	return true;
}
bool ComaWindow::setCursor(HCURSOR cursor)
{
	if (isCreated())
	{
		SetCursor(cursor);
	}
	else
	{
		windowData.cursor = cursor;
	}
	return true;
}
bool ComaWindow::setBackgroundColor(HBRUSH brush)
{
	if (isCreated())
		return false;
	windowData.backgroundColor = brush;
	return true;
}//수정 필요
bool ComaWindow::setStyle(DWORD style)
{
	if (isCreated())
	{
		SetWindowLong(hWnd, GWL_STYLE, style);
	}
	else
	{
		windowData.style = style;
	}
	return true;
}
bool ComaWindow::setStyleEx(DWORD styleEx)
{
	if (isCreated())
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, styleEx);
	}
	else
	{
		windowData.style = styleEx;
	}
	return true;
}
bool ComaWindow::setTitle(TCHAR* title)
{
	if (isCreated())
	{
		if (SetWindowText(hWnd, title))
			return true;
		return false;
	}
	else
	{
		windowData.title = title;
	}
	return true;
}
bool ComaWindow::setCmdShow(int nCmdShow)
{
	if (isCreated())
		return false;
	windowData.nCmdShow = nCmdShow;
	return true;
}

bool ComaWindow::setWindowRect(RECT rect)
{
	if (isCreated())
	{
		if (SetWindowPos(hWnd, NULL, rect.left, rect.top, rect.right, rect.bottom, 0))
			return true;
		return false;
	}
	else
	{
		windowData.windowPosition = { rect.left, rect.top };
		if (rect.left == CW_USEDEFAULT)
			rect.left = 0;
		if (rect.top == CW_USEDEFAULT)
			rect.top = 0;
		windowData.windowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	}
	return true;
}
bool ComaWindow::setWindowRect(int left, int top, int right, int bottom)
{
	return setWindowRect(RECT{ left, top, right, bottom });
}
bool ComaWindow::setWindowSize(RECT rect)
{
	if (isCreated())
	{
		POINT position = getWindowPosition();
		if (SetWindowPos(hWnd, NULL, position.x, position.y, rect.right - rect.left, rect.bottom - rect.top, 0))
			return true;
		return false;
	}
	else
	{
		windowData.windowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	}
	return true;
}
bool ComaWindow::setWindowSize(int width, int height)
{
	return setWindowSize(RECT{ 0, 0, width, height });
}
bool ComaWindow::setWindowPosition(POINT point)
{
	if (isCreated())
	{
		RECT r = getWindowSize();
		if (SetWindowPos(hWnd, NULL, point.x, point.y, r.right - r.left, r.bottom - r.top, 0))
			return true;
		return false;
	}
	else
	{
		windowData.windowPosition = { point.x, point.y };
	}
	return true;
}
bool ComaWindow::setWindowPosition(int x, int y)
{
	return setWindowPosition(POINT{ x, y });
}
bool ComaWindow::setScreenSize(RECT rect)
{
	if (!AdjustWindowRectEx(&rect, getStyle(), false, getStyleEx()))
		return false;
	return setWindowSize(rect);
}
bool ComaWindow::setScreenSize(int width, int height)
{
	return setScreenSize(RECT{ 0, 0, width, height });
}


bool ComaWindow::setMaxWindowSize(RECT rect)
{
	windowData.maxWindowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	return true;
}
bool ComaWindow::setMaxWindowSize(int width, int height)
{
	return setMaxWindowSize(RECT{ 0, 0, width, height });
}
bool ComaWindow::setMaxScreenSize(RECT rect)
{
	if (!AdjustWindowRectEx(&rect, getStyle(), false, getStyleEx()))
		return false;
	windowData.maxWindowSize = rect;
	return true;
}
bool ComaWindow::setMaxScreenSize(int width, int height)
{
	return setMaxScreenSize(RECT{ 0, 0, width, height });
}
bool ComaWindow::setMinWindowSize(RECT rect)
{
	windowData.minWindowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	return true;
}
bool ComaWindow::setMinWindowSize(int width, int height)
{
	return setMinWindowSize(RECT{ 0, 0, width, height });
}
bool ComaWindow::setMinScreenSize(RECT rect)
{
	if (!AdjustWindowRectEx(&rect, getStyle(), false, getStyleEx()))
		return false;
	windowData.minWindowSize = rect;
	return true;
}
bool ComaWindow::setMinScreenSize(int width, int height)
{
	return setMinScreenSize(RECT{ 0, 0, width, height });
}


bool ComaWindow::addStyle(DWORD style)
{
	DWORD st = getStyle();
	st |= style;
	return setStyle(st);
}
bool ComaWindow::removeStyle(DWORD style)
{
	DWORD st = getStyle();
	st &= ~style;
	return setStyle(st);
}
bool ComaWindow::addStyleEx(DWORD styleEx)
{
	DWORD st = getStyleEx();
	st |= styleEx;
	return setStyleEx(st);
}
bool ComaWindow::removeStyleEx(DWORD styleEx)
{
	DWORD st = getStyleEx();
	st &= ~styleEx;
	return setStyleEx(st);
}

//Getter
DWORD	ComaWindow::getStyle()
{
	if (isCreated())
	{
		return GetWindowLong(hWnd, GWL_STYLE);
	}
	return windowData.style;
}
DWORD	ComaWindow::getStyleEx()
{
	if (isCreated())
	{
		return GetWindowLong(hWnd, GWL_EXSTYLE);
	}
	return windowData.styleEx;
}
TCHAR*	ComaWindow::getTitle()
{
	if (isCreated())
	{
		TCHAR* title = new TCHAR[256];
		GetWindowText(hWnd, title, 256);
		return title;
	}
	return windowData.title;
}
RECT	ComaWindow::getWindowSize()
{
	if (isCreated())
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return RECT{ 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	}
	return windowData.windowSize;
}
RECT	ComaWindow::getWindowRect()
{
	if (isCreated())
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return rect;
	}
	return RECT{ 
		windowData.windowPosition.x,
		windowData.windowPosition.y,
		windowData.windowPosition.x + windowData.windowSize.right - windowData.windowSize.left,
		windowData.windowPosition.y + windowData.windowSize.bottom - windowData.windowSize.top
	};
}
RECT	ComaWindow::getScreenSize()
{
	RECT rect;
	if (isCreated())
	{
		GetClientRect(hWnd, &rect);
		return rect;
	}
	rect = { 0, 0, 0, 0 };
	AdjustWindowRectEx(&rect, getStyle(), false, getStyleEx());
	return RECT { 
		windowData.windowSize.left - rect.left,
		windowData.windowSize.top - rect.top,
		windowData.windowSize.right - rect.right,
		windowData.windowSize.bottom - rect.bottom
	};
}
POINT	ComaWindow::getWindowPosition()
{
	if (isCreated())
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return POINT{ rect.left, rect.top };
	}
	return windowData.windowPosition;
}

//Status Changer
bool ComaWindow::setFullscreen()
{
	return false;//TODO: fullscreen 구현
}
bool ComaWindow::minimizeWindow()
{
	if (isCreated())
	{
		if (!PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE, 0))
			return false;
		return true;
	}
	else
	{
		minimized = true;
		maximized = false;
	}
	return true;
}
bool ComaWindow::maximizeWindow()
{
	if (isCreated())
	{
		if (!PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_MAXIMIZE, 0))
			return false;
		return true;
	}
	else
	{
		minimized = false;
		maximized = true;
	}
	return true;
}
bool ComaWindow::restoreWindow()
{
	if (isCreated())
	{
		if (!PostMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0))
			return false;
		return true;
	}
	else
	{
		minimized = false;
		maximized = false;
	}
	return true;
}

//TODO: 이벤트 리스너 구현