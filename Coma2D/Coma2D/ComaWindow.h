/*
* filename	ComaWindow.h
* fileinfo	윈도우 생성 및 관리 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once

#include "EventDispatcher.h"
#include "ComaRenderer.h"
#include "InputManager.h"

COMA_NS_BEGIN

struct WindowData
{
	HICON icon;
	HCURSOR cursor;
	HBRUSH backgroundColor;
	DWORD style;
	DWORD styleEx;
	TCHAR* title;
	int nCmdShow;
	POINT windowPosition;
	RECT windowSize;
	RECT maxWindowSize;
	RECT minWindowSize;
	RECT fullscreenSize;
};

class ComaWindow :
	public EventDispatcher
{
public:
	ComaWindow(HINSTANCE hInstance, int nCmdShow = SW_SHOW);
	virtual ~ComaWindow();

	//Window Initialize
	bool CreateComaWindow();
	bool Run();

	LRESULT MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	HINSTANCE GetInstance() { return hInstance_; }
	HWND GetWindow() { return hWnd_; }
	InputManager* GetInputManager() { return inputManager_; }

	DWORD GetStyle();
	DWORD GetStyleEx();
	TCHAR* GetTitle();
	RECT GetWindowSize();
	RECT GetWindowSizeRect();
	RECT GetScreenSize();
	POINT GetWindowPosition();

	//Window Setter
	bool SetIcon(HICON icon);
	bool SetCursor(HCURSOR cursor);
	bool SetBackgroundColor(HBRUSH brush);
	bool SetStyle(DWORD style);
	bool SetStyleEx(DWORD styleEx);
	bool SetTitle(TCHAR* title);
	bool SetCmdShow(int nCmdShow);


	bool SetWindowRect(RECT rect);
	bool SetWindowRect(int left, int top, int right, int bottom);
	bool SetWindowSize(RECT rect);
	bool SetWindowSize(int width, int height);
	bool SetWindowPosition(POINT point);
	bool SetWindowPosition(int x, int y);
	bool SetScreenSize(RECT rect);
	bool SetScreenSize(int width, int height);


	bool SetMaxScreenSize(RECT rect);
	bool SetMaxScreenSize(int width, int height);
	bool SetMaxWindowSize(RECT rect);
	bool SetMaxWindowSize(int width, int height);
	bool SetMinScreenSize(RECT rect);
	bool SetMinScreenSize(int width, int height);
	bool SetMinWindowSize(RECT rect);
	bool SetMinWindowSize(int width, int height);

	bool AddStyle(DWORD style);
	bool RemoveStyle(DWORD style);
	bool AddStyleEx(DWORD styleEx);
	bool RemoveStyleEx(DWORD styleEx);

	//Window Status Getter
	bool IsCreated() { return created_; }
	bool IsRunning() { return running_; }
	bool IsActivated() { return activated_; }
	bool IsFullscreen() { return fullscreen_; }
	bool IsMaximized() { return maximized_; }
	bool IsMinimized() { return minimized_; }
	bool IsResizing() { return resizing_; }
	bool IsMoving() { return moving_; }

	//Window Status Changer
	bool SetFullscreen(bool mode);
	bool SetFullscreen(bool mode, int width, int height);
	bool MinimizeWindow();
	bool MaximizeWindow();
	bool RestoreWindow();

private:
	//Window Essencials
	HINSTANCE hInstance_;
	HWND hWnd_;
	WindowData windowData_;

	//Manager
	InputManager* inputManager_;

	//Window Status
	bool created_;
	bool running_;
	bool activated_;
	bool fullscreen_;
	bool maximized_;
	bool minimized_;
	bool resizing_;
	bool moving_;
};

COMA_END

