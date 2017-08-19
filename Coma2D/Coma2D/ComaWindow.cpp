/*
* filename	ComaWindow.cpp
* fileinfo	윈도우 생성 및 관리 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "ComaWindow.h"
#include "WindowEvent.h"
#include <tchar.h>
COMA_USING_NS

#define TITLE_LIMIT 256

ComaWindow* comaWindow;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return comaWindow->MessageProc(hWnd, uMsg, wParam, lParam);
}

ComaWindow::ComaWindow(HINSTANCE hInstance, int nCmdShow)
	:hInstance_(hInstance), hWnd_(0), created_(false), running_(false), activated_(false), fullscreen_(false), maximized_(false), minimized_(false), resizing_(false), inputManager_(nullptr)
{
	comaWindow = this;

	windowData_.icon = LoadIcon(0, IDI_APPLICATION);
	windowData_.cursor = LoadCursor(0, IDC_ARROW);
	windowData_.backgroundColor = (HBRUSH)GetStockObject(NULL_BRUSH);
	windowData_.style = WS_OVERLAPPEDWINDOW;
	windowData_.styleEx = NULL;
	windowData_.title = new TCHAR[TITLE_LIMIT];
	SetTitle(TEXT("ComaWindow"));
	windowData_.nCmdShow = nCmdShow;
	windowData_.windowPosition = { CW_USEDEFAULT, CW_USEDEFAULT };
	windowData_.windowSize = { 0, 0, CW_USEDEFAULT, CW_USEDEFAULT };
	windowData_.minWindowSize = { 0, 0, -1, -1 };
	windowData_.maxWindowSize = { 0, 0, -1, -1 };
}

ComaWindow::~ComaWindow()
{
	if (windowData_.title) delete[] windowData_.title;
	if (inputManager_) delete inputManager_;
	DestroyWindow(hWnd_);
}

bool ComaWindow::CreateComaWindow()
{
	if (IsCreated())
	{
		return false;
	}

	TCHAR* className = TEXT("ComaWindow");
	WNDCLASS windowClass;

	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance_;
	windowClass.hIcon = windowData_.icon;
	windowClass.hCursor = windowData_.cursor;
	windowClass.hbrBackground = windowData_.backgroundColor;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;

	if (!RegisterClass(&windowClass))
	{
		return false;
	}

	hWnd_ = CreateWindowEx(
		windowData_.styleEx, className,
		windowData_.title,
		windowData_.style,
		windowData_.windowPosition.x, windowData_.windowPosition.y,
		windowData_.windowSize.right - windowData_.windowSize.left,
		windowData_.windowSize.bottom - windowData_.windowSize.top,
		0, 0, hInstance_, 0);

	if (!hWnd_)
	{
		return false;
	}

	created_ = true;

	inputManager_ = new InputManager();

	if (IsMaximized())
		windowData_.nCmdShow = SW_SHOWMAXIMIZED;
	else if (IsMinimized())
		windowData_.nCmdShow = SW_SHOWMINIMIZED;

	ShowWindow(hWnd_, windowData_.nCmdShow);
	UpdateWindow(hWnd_);

	if (IsFullscreen())
	{
		fullscreen_ = false;
		SetFullscreen(true);
	}

	DispatchEvent(new WindowEvent(WindowEvent::CREATED, this));
	return true;
}

// 윈도우 루프
bool ComaWindow::Run()
{
	if (!hWnd_ || IsRunning())
	{
		return false;
	}
	running_ = true;

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
			DispatchEvent(new WindowEvent(WindowEvent::UPDATE, this));
		}
	}
	return true;
}

// 메세지 처리부
LRESULT ComaWindow::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			activated_ = false;
			if (IsFullscreen())
			{
				ChangeDisplaySettings(NULL, 0);
				fullscreen_ = false;
				MinimizeWindow();
				fullscreen_ = true;
			}
			DispatchEvent(new WindowEvent(WindowEvent::INACTIVATED, this, wParam, lParam));
		}
		else if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
		{
			activated_ = true;
			if (IsFullscreen())
			{
				fullscreen_ = false;
				SetFullscreen(true);
			}
			DispatchEvent(new WindowEvent(WindowEvent::ACTIVATED, this, wParam, lParam));
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		DispatchEvent(new WindowEvent(WindowEvent::ENTER_RESIZEMOVE, this, wParam, lParam));
		return 0;

	case WM_EXITSIZEMOVE:
		moving_ = false;
		resizing_ = false;
		DispatchEvent(new WindowEvent(WindowEvent::EXIT_RESIZEMOVE, this, wParam, lParam));
		return 0;

	case WM_MOVE:
		DispatchEvent(new WindowEvent(WindowEvent::MOVE, this, wParam, lParam));
		return 0;

	case WM_MOVING:
		moving_ = true;
		DispatchEvent(new WindowEvent(WindowEvent::MOVING, this, wParam, lParam));
		return 0;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			minimized_ = true;
			maximized_ = false;
			resizing_ = false;
			DispatchEvent(new WindowEvent(WindowEvent::MINIMIZED, this, wParam, lParam));
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			minimized_ = false;
			maximized_ = true;
			resizing_ = false;
			DispatchEvent(new WindowEvent(WindowEvent::MAXIMIZED, this, wParam, lParam));
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (maximized_)
			{
				maximized_ = false;
				minimized_ = false;
				resizing_ = false;
				DispatchEvent(new WindowEvent(WindowEvent::RESTORED, this, wParam, lParam));
			}
			else if (minimized_)
			{
				maximized_ = false;
				minimized_ = false;
				resizing_ = false;
				DispatchEvent(new WindowEvent(WindowEvent::RESTORED, this, wParam, lParam));
			}
			else if (resizing_)
			{

			}
		}
		DispatchEvent(new WindowEvent(WindowEvent::RESIZE, this, wParam, lParam));
		return 0;

	case WM_SIZING:
		resizing_ = true;
		DispatchEvent(new WindowEvent(WindowEvent::RESIZING, this, wParam, lParam));
		return 0;

	case WM_GETMINMAXINFO:
		if (windowData_.maxWindowSize.right - windowData_.maxWindowSize.left > 0)
			((MINMAXINFO*)lParam)->ptMaxTrackSize.x = windowData_.maxWindowSize.right - windowData_.maxWindowSize.left;
		if (windowData_.maxWindowSize.bottom - windowData_.maxWindowSize.top > 0)
			((MINMAXINFO*)lParam)->ptMaxTrackSize.y = windowData_.maxWindowSize.bottom - windowData_.maxWindowSize.top;
		if (windowData_.minWindowSize.right - windowData_.minWindowSize.left > 0)
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = windowData_.minWindowSize.right - windowData_.minWindowSize.left;
		if (windowData_.minWindowSize.bottom - windowData_.minWindowSize.top > 0)
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = windowData_.minWindowSize.bottom - windowData_.minWindowSize.top;
		return 0;

	case WM_SYSCOMMAND:
		if (LOWORD(wParam) == SC_MINIMIZE)
		{
			windowData_.windowSize = GetWindowSize();
			windowData_.windowPosition = GetWindowPosition();
			//최소화되기 전의 화면 크기를 저장.
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	case WM_DESTROY:
		DispatchEvent(new WindowEvent(WindowEvent::DESTROY, this, wParam, lParam));
		PostQuitMessage(0);
		created_ = false;
		running_ = false;
		activated_ = false;
		fullscreen_ = false;
		maximized_ = false;
		minimized_ = false;
		resizing_ = false;
		return 0;
	}

	if (inputManager_->CreateInputEvent(uMsg, wParam, lParam))
		return 0;

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool ComaWindow::SetIcon(HICON icon)
{
	if (IsCreated())
	{
		return false;
	}
	windowData_.icon = icon;
	return true;
}

bool ComaWindow::SetCursor(HCURSOR cursor)
{
	if (IsCreated())
	{
		SetCursor(cursor);
	}
	else
	{
		windowData_.cursor = cursor;
	}
	return true;
}

bool ComaWindow::SetBackgroundColor(HBRUSH brush)
{
	if (IsCreated())
	{
		return false;
	}
	windowData_.backgroundColor = brush;
	return true;
}//수정 필요

bool ComaWindow::SetStyle(DWORD style)
{
	if (IsCreated() && !IsFullscreen())
	{
		SetWindowLong(hWnd_, GWL_STYLE, style);
	}
	else
	{
		windowData_.style = style;
	}
	return true;
}

bool ComaWindow::SetStyleEx(DWORD styleEx)
{
	if (IsCreated() && !IsFullscreen())
	{
		SetWindowLong(hWnd_, GWL_EXSTYLE, styleEx);
	}
	else
	{
		windowData_.style = styleEx;
	}
	return true;
}

bool ComaWindow::SetTitle(const TCHAR* title)
{
	if (IsCreated())
	{
		return SetWindowText(hWnd_, title);
	}
	else
	{
		_tcscpy_s(windowData_.title, TITLE_LIMIT, title);
	}
	return true;
}

bool ComaWindow::SetCmdShow(int nCmdShow)
{
	if (IsCreated())
	{
		return false;
	}
	windowData_.nCmdShow = nCmdShow;
	return true;
}

bool ComaWindow::SetWindowRect(RECT rect)
{
	if (IsCreated() && !IsFullscreen() && !IsMinimized())
	{
		return SetWindowPos(hWnd_, NULL, rect.left, rect.top, rect.right, rect.bottom, 0);
	}
	else
	{
		windowData_.windowPosition = { rect.left, rect.top };
		if (rect.left == CW_USEDEFAULT)
		{
			rect.left = 0;
		}
		if (rect.top == CW_USEDEFAULT)
		{
			rect.top = 0;
		}
		windowData_.windowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	}
	return true;
}

bool ComaWindow::SetWindowRect(int left, int top, int right, int bottom)
{
	return SetWindowRect(RECT{ left, top, right, bottom });
}

bool ComaWindow::SetWindowSize(RECT rect)
{
	if (IsCreated() && !IsFullscreen() && !IsMinimized())
	{
		POINT position = GetWindowPosition();
		return SetWindowPos(hWnd_, NULL, position.x, position.y, rect.right - rect.left, rect.bottom - rect.top, 0);
	}
	else
	{
		windowData_.windowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	}
	return true;
}

bool ComaWindow::SetWindowSize(int width, int height)
{
	return SetWindowSize(RECT{ 0, 0, width, height });
}

bool ComaWindow::SetWindowPosition(POINT point)
{
	if (IsCreated() && !IsFullscreen() && !IsMinimized())
	{
		RECT r = GetWindowSize();
		return SetWindowPos(hWnd_, NULL, point.x, point.y, r.right - r.left, r.bottom - r.top, 0);
	}
	else
	{
		windowData_.windowPosition = { point.x, point.y };
	}
	return true;
}

bool ComaWindow::SetWindowPosition(int x, int y)
{
	return SetWindowPosition(POINT{ x, y });
}

bool ComaWindow::SetScreenSize(RECT rect)
{
	if (!AdjustWindowRectEx(&rect, GetStyle(), false, GetStyleEx()))
		return false;
	return SetWindowSize(rect);
}

bool ComaWindow::SetScreenSize(int width, int height)
{
	return SetScreenSize(RECT{ 0, 0, width, height });
}


bool ComaWindow::SetMaxWindowSize(RECT rect)
{
	windowData_.maxWindowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	return true;
}

bool ComaWindow::SetMaxWindowSize(int width, int height)
{
	return SetMaxWindowSize(RECT{ 0, 0, width, height });
}

bool ComaWindow::SetMaxScreenSize(RECT rect)
{
	RECT outRect = rect;
	if (!AdjustWindowRectEx(&outRect, GetStyle(), false, GetStyleEx()))
	{
		return false;
	}

	if (rect.right - rect.left < 0)
	{
		outRect.right = -1;
		outRect.left = 0;
	}

	if (rect.bottom - rect.top < 0)
	{
		outRect.bottom = -1;
		outRect.top = 0;
	}

	windowData_.maxWindowSize = outRect;
	return true;
}

bool ComaWindow::SetMaxScreenSize(int width, int height)
{
	return SetMaxScreenSize(RECT{ 0, 0, width, height });
}

bool ComaWindow::SetMinWindowSize(RECT rect)
{
	windowData_.minWindowSize = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	return true;
}

bool ComaWindow::SetMinWindowSize(int width, int height)
{
	return SetMinWindowSize(RECT{ 0, 0, width, height });
}

bool ComaWindow::SetMinScreenSize(RECT rect)
{
	RECT outRect = rect;
	if (!AdjustWindowRectEx(&outRect, GetStyle(), false, GetStyleEx()))
	{
		return false;
	}
	if (rect.right - rect.left < 0)
	{
		outRect.right = -1;
		outRect.left = 0;
	}
	if (rect.bottom - rect.top < 0)
	{
		outRect.bottom = -1;
		outRect.top = 0;
	}
	windowData_.minWindowSize = outRect;
	return true;
}

bool ComaWindow::SetMinScreenSize(int width, int height)
{
	return SetMinScreenSize(RECT{ 0, 0, width, height });
}

bool ComaWindow::AddStyle(DWORD style)
{
	DWORD st = GetStyle();
	st |= style;
	return SetStyle(st);
}

bool ComaWindow::RemoveStyle(DWORD style)
{
	DWORD st = GetStyle();
	st &= ~style;
	return SetStyle(st);
}

bool ComaWindow::AddStyleEx(DWORD styleEx)
{
	DWORD st = GetStyleEx();
	st |= styleEx;
	return SetStyleEx(st);
}

bool ComaWindow::RemoveStyleEx(DWORD styleEx)
{
	DWORD st = GetStyleEx();
	st &= ~styleEx;
	return SetStyleEx(st);
}

DWORD ComaWindow::GetStyle() const
{
	if (IsCreated())
	{
		return GetWindowLong(hWnd_, GWL_STYLE);
	}
	return windowData_.style;
}

DWORD ComaWindow::GetStyleEx() const
{
	if (IsCreated())
	{
		return GetWindowLong(hWnd_, GWL_EXSTYLE);
	}
	return windowData_.styleEx;
}

const TCHAR* ComaWindow::GetTitle()
{
	if (IsCreated())
	{
		if (windowData_.title)
		{
			delete windowData_.title;
		}

		windowData_.title = new TCHAR[256];
		GetWindowText(hWnd_, windowData_.title, 256);
	}
	return windowData_.title;
}

RECT ComaWindow::GetWindowSize() const
{
	if (IsCreated() && !IsMinimized())
	{
		RECT rect;
		GetWindowRect(hWnd_, &rect);
		return RECT{ 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	}
	return windowData_.windowSize;
}

RECT ComaWindow::GetWindowSizeRect() const
{
	if (IsCreated() && !IsMinimized())
	{
		RECT rect;
		GetWindowRect(hWnd_, &rect);
		return rect;
	}
	return RECT{
		windowData_.windowPosition.x,
		windowData_.windowPosition.y,
		windowData_.windowPosition.x + windowData_.windowSize.right - windowData_.windowSize.left,
		windowData_.windowPosition.y + windowData_.windowSize.bottom - windowData_.windowSize.top
	};
}

RECT ComaWindow::GetScreenSize() const
{
	RECT rect;
	if (IsCreated() && !IsMinimized())
	{
		GetClientRect(hWnd_, &rect);
		return rect;
	}
	rect = { 0, 0, 0, 0 };
	AdjustWindowRectEx(&rect, GetStyle(), false, GetStyleEx());
	return RECT{
		windowData_.windowSize.left - rect.left,
		windowData_.windowSize.top - rect.top,
		windowData_.windowSize.right - rect.right,
		windowData_.windowSize.bottom - rect.bottom
	};
}//수정 필요

POINT ComaWindow::GetWindowPosition() const
{
	if (IsCreated() && !IsMinimized())
	{
		RECT rect;
		GetWindowRect(hWnd_, &rect);
		return POINT{ rect.left, rect.top };
	}
	return windowData_.windowPosition;
}

//Status Changer
bool ComaWindow::SetFullscreen(bool mode)
{
	if (!mode)
	{
		if (!IsFullscreen())
		{
			return false;
		}

		fullscreen_ = false;
		ChangeDisplaySettings(NULL, 0);
		SetStyle(windowData_.style);
		SetStyleEx(windowData_.styleEx);
		SetWindowPosition(windowData_.windowPosition);
		SetWindowSize(windowData_.windowSize);

		if (IsMaximized())
		{
			MaximizeWindow();
		}
		else if (IsMinimized())
		{
			MinimizeWindow();
		}

		fullscreen_ = false;
		DispatchEvent(new WindowEvent(WindowEvent::EXIT_FULLSCREEN, this));
		return true;
	}

	RECT rect = GetScreenSize();
	if (SetFullscreen(mode, rect.right, rect.bottom))
	{
		return true;
	}
	if (SetFullscreen(mode, windowData_.fullscreenSize.right, windowData_.fullscreenSize.bottom))
	{
		return true;
	}
	return false;
}

bool ComaWindow::SetFullscreen(bool mode, int width, int height)
{
	if (!IsCreated())
	{
		fullscreen_ = true;
		windowData_.fullscreenSize = { 0,0, width, height };
		return true;
	}
	if (IsFullscreen())
	{
		return false;
	}

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
		return false;
	}
	windowData_.fullscreenSize = { 0, 0, width, height };
	windowData_.style = GetStyle();
	windowData_.styleEx = GetStyleEx();
	windowData_.windowSize = GetWindowSize();
	windowData_.windowPosition = GetWindowPosition();
	SetStyle(windowData_.style & ~WS_OVERLAPPEDWINDOW | WS_POPUP);
	SetStyleEx(windowData_.styleEx | WS_EX_TOPMOST);
	SetWindowSize(width, height);
	SetWindowPosition(0, 0);
	fullscreen_ = true;
	DispatchEvent(new WindowEvent(WindowEvent::ENTER_FULLSCREEN, this));

	return true;
}

bool ComaWindow::MinimizeWindow()
{
	if (IsCreated() && !IsFullscreen())
	{
		return PostMessage(hWnd_, WM_SYSCOMMAND, (WPARAM)SC_MINIMIZE, 0);
	}
	else
	{
		minimized_ = true;
		maximized_ = false;
	}
	return true;
}

bool ComaWindow::MaximizeWindow()
{
	if (IsCreated() && !IsFullscreen())
	{
		return PostMessage(hWnd_, WM_SYSCOMMAND, (WPARAM)SC_MAXIMIZE, 0);
	}
	else
	{
		minimized_ = false;
		maximized_ = true;
	}
	return true;
}

bool ComaWindow::RestoreWindow()
{
	if (IsCreated() && !IsFullscreen())
	{
		return PostMessage(hWnd_, WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
	}
	else
	{
		minimized_ = false;
		maximized_ = false;
	}
	return true;
}
