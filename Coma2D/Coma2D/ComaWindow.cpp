/*
* The MIT License (MIT)
*
* Copyright (c) 2015 Heonyang Ju
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
* MIT 라이선스 (MIT)
*
* 저작권(C) 2015 주헌양
*
* 이 문서에 의해, 아래의 조건을 따르는 한 제한 없이 소프트웨어를 다루기 위해
* 소프트웨어의 복사본과 관련된 문서("소프트웨어")를 획득하는 사람에게 변경의
* 자유가 허용 된다. 제한 없음은 사용, 복제, 수정, 통합, 공포, 배포, 하위 라이선스
* 그리고/또는 소프트웨어의 복사본의 판매와 공급된 소프트웨어를 가진 사람에게
* 그렇게 할 수 있도록 허가하는 권한에 제한 없음을 포함 한다.
*
* 위의 저작권 표시와 이 허가 표시는 모든 소프트웨어의 복사본 또는 상당한 일부분에
* 포함되어야 한다.
*
* 이 소프트웨어는 특정한 목적과 침해 없음을 위해 상업성, 적합성의 보증에만
* 국한되는 것이 아닌 명시적 또는 묵시적인 어떠한 종류의 보증없이 "있는 그대로"
* 제공 된다. 어떠한 경우에도 저작자 또는 저작권자는 소프트웨어의 관계 안과 밖
* 또는 소프트웨어의 사용 또는 거래로부터 발생하는 계약, 불법 행위 또는 기타 수행에
* 대해, 어떠한 소송, 손해 또는 다른 책무에 대해서도 책임이 없다.
*
* 본문은 MIT라이선스 원문의 번역본이며, 모든 해석은 원문에 기초한다.
*/

/*
* filename	ComaWindow.cpp
* fileinfo	윈도우 생성 및 관리 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "ComaWindow.h"
#include "WindowEvent.h"
COMA_USING_NS

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
	maximized(false), minimized(false), resizing(false), inputManager(nullptr)
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
	if(inputManager) delete inputManager;
	DestroyWindow(hWnd);
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

	inputManager = new InputManager();

	if (isMaximized())
		windowData.nCmdShow = SW_SHOWMAXIMIZED;
	else if (isMinimized())
		windowData.nCmdShow = SW_SHOWMINIMIZED;

	//윈도우 출력
	ShowWindow(hWnd, windowData.nCmdShow);
	UpdateWindow(hWnd);

	if (isFullscreen())
	{
		fullscreen = false;
		setFullscreen(true);
	}
	dispatchEvent(new WindowEvent(WindowEvent::CREATED, this));
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
			dispatchEvent(new WindowEvent(WindowEvent::UPDATE, this));
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
			if (isFullscreen())
			{
				ChangeDisplaySettings(NULL, 0);
				fullscreen = false;
				minimizeWindow();
				fullscreen = true;
			}
			dispatchEvent(new WindowEvent(WindowEvent::INACTIVATED, this, wParam, lParam));
		}
		else if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
		{
			activated = true;
			if (isFullscreen())
			{
				fullscreen = false;
				setFullscreen(true);
			}
			dispatchEvent(new WindowEvent(WindowEvent::ACTIVATED, this, wParam, lParam));
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		dispatchEvent(new WindowEvent(WindowEvent::ENTER_RESIZEMOVE, this, wParam, lParam));
		return 0;

	case WM_EXITSIZEMOVE:
		moving = false;
		resizing = false;
		dispatchEvent(new WindowEvent(WindowEvent::EXIT_RESIZEMOVE, this, wParam, lParam));
		return 0;

	case WM_MOVE:
		dispatchEvent(new WindowEvent(WindowEvent::MOVE, this, wParam, lParam));
		return 0;

	case WM_MOVING:
		moving = true;
		dispatchEvent(new WindowEvent(WindowEvent::MOVING, this, wParam, lParam));
		return 0;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			minimized = true;
			maximized = false;
			resizing = false;
			dispatchEvent(new WindowEvent(WindowEvent::MINIMIZED, this, wParam, lParam));
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			minimized = false;
			maximized = true;
			resizing = false;
			dispatchEvent(new WindowEvent(WindowEvent::MAXIMIZED, this, wParam, lParam));
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (maximized)
			{
				maximized = false;
				minimized = false;
				resizing = false;
				dispatchEvent(new WindowEvent(WindowEvent::RESTORED, this, wParam, lParam));
			}
			else if (minimized)
			{
				maximized = false;
				minimized = false;
				resizing = false;
				dispatchEvent(new WindowEvent(WindowEvent::RESTORED, this, wParam, lParam));
			}
			else if (resizing)
			{
				
			}
		}
		dispatchEvent(new WindowEvent(WindowEvent::RESIZE, this, wParam, lParam));
		return 0;

	case WM_SIZING:
		resizing = true;
		dispatchEvent(new WindowEvent(WindowEvent::RESIZING, this, wParam, lParam));
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
	case WM_SYSCOMMAND:
		if (LOWORD(wParam) == SC_MINIMIZE)
		{
			windowData.windowSize = getWindowSize();
			windowData.windowPosition = getWindowPosition();
			//최소화되기 전에, 화면 크기를 저장해둔다.
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	case WM_DESTROY:
		dispatchEvent(new WindowEvent(WindowEvent::DESTROY, this, wParam, lParam));
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

	if (inputManager->createInputEvent(uMsg, wParam, lParam))
		return 0;

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
	if (isCreated() && !isFullscreen())
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
	if (isCreated() && !isFullscreen())
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
	if (isCreated() && !isFullscreen() && !isMinimized())
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
	if (isCreated() && !isFullscreen() && !isMinimized())
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
	if (isCreated() && !isFullscreen() && !isMinimized())
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
	RECT outRect = rect;
	if (!AdjustWindowRectEx(&outRect, getStyle(), false, getStyleEx()))
		return false;
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
	windowData.maxWindowSize = outRect;
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
	RECT outRect = rect;
	if (!AdjustWindowRectEx(&outRect, getStyle(), false, getStyleEx()))
		return false;
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
	windowData.minWindowSize = outRect;
	return true;
}
bool ComaWindow::setMinScreenSize(int width, int height)
{
	return setMinScreenSize(RECT{ 0, 0, width, height });
}
//입력값이 음수이면 최대, 최소 크기를 검사하지 않는다.

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
	if (isCreated() && !isMinimized())
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return RECT{ 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	}
	return windowData.windowSize;
}
RECT	ComaWindow::getWindowRect()
{
	if (isCreated() && !isMinimized())
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
	if (isCreated() && !isMinimized())
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
}//수정 필요
POINT	ComaWindow::getWindowPosition()
{
	if (isCreated() && !isMinimized())
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return POINT{ rect.left, rect.top };
	}
	return windowData.windowPosition;
}

//Status Changer
bool ComaWindow::setFullscreen(bool mode)
{
	if (!mode)
	{
		if (!isFullscreen())
			return false;

		fullscreen = false;
		ChangeDisplaySettings(NULL, 0);
		setStyle(windowData.style);
		setStyleEx(windowData.styleEx);
		setWindowPosition(windowData.windowPosition);
		setWindowSize(windowData.windowSize);
		if (isMaximized())
			maximizeWindow();
		else if (isMinimized())
			minimizeWindow();

		fullscreen = false;
		dispatchEvent(new WindowEvent(WindowEvent::EXIT_FULLSCREEN, this));
		return true;
	} //풀스크린 해제

	RECT rect = getScreenSize();
	if (setFullscreen(mode, rect.right, rect.bottom))
		return true;
	if (setFullscreen(mode, windowData.fullscreenSize.right, windowData.fullscreenSize.bottom))
		return true;
	//다양한 해상도로 시도
	return false;
}
bool ComaWindow::setFullscreen(bool mode, int width ,int height)
{
	if (!isCreated())
	{
		fullscreen = true;
		windowData.fullscreenSize = {0,0, width, height };
		return true;
	}
	if (isFullscreen())
		return false;

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
	windowData.fullscreenSize = { 0, 0, width, height };
	windowData.style = getStyle();
	windowData.styleEx = getStyleEx();
	windowData.windowSize = getWindowSize();
	windowData.windowPosition = getWindowPosition();
	setStyle(windowData.style & ~WS_OVERLAPPEDWINDOW | WS_POPUP);
	setStyleEx(windowData.styleEx | WS_EX_TOPMOST);
	setWindowSize(width, height);
	setWindowPosition(0, 0);
	fullscreen = true;
	dispatchEvent(new WindowEvent(WindowEvent::ENTER_FULLSCREEN, this));
	return true;
}
bool ComaWindow::minimizeWindow()
{
	if (isCreated() && !isFullscreen())
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
	if (isCreated() && !isFullscreen())
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
	if (isCreated() && !isFullscreen())
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
