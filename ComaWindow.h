#pragma once
#include "EventDispatcher.h"
#include "ComaRenderer.h"
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
	bool	createWindow();
	bool	run();

	//Window Controls


	//Window Essencials
	LRESULT messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	//Window Getter
	HINSTANCE	getInstance() { return hInstance; }
	HWND		getWindow(){ return hWnd; }
	
	DWORD	getStyle();
	DWORD	getStyleEx();
	TCHAR*	getTitle();
	RECT	getWindowSize();
	RECT	getWindowRect();
	RECT	getScreenSize();
	POINT	getWindowPosition();

	//Window Setter
	bool setIcon(HICON icon);
	bool setCursor(HCURSOR cursor);
	bool setBackgroundColor(HBRUSH brush);
	bool setStyle(DWORD style);
	bool setStyleEx(DWORD styleEx);
	bool setTitle(TCHAR* title);
	bool setCmdShow(int nCmdShow);

	bool setWindowRect(RECT rect);
	bool setWindowRect(int left, int top, int right, int bottom);
	bool setWindowSize(RECT rect);
	bool setWindowSize(int width, int height);
	bool setWindowPosition(POINT point);
	bool setWindowPosition(int x, int y);
	bool setScreenSize(RECT rect);
	bool setScreenSize(int width, int height);

	bool setMaxScreenSize(RECT rect);
	bool setMaxScreenSize(int width, int height);
	bool setMaxWindowSize(RECT rect);
	bool setMaxWindowSize(int width, int height);
	bool setMinScreenSize(RECT rect);
	bool setMinScreenSize(int width, int height);
	bool setMinWindowSize(RECT rect);
	bool setMinWindowSize(int width, int height);

	bool addStyle(DWORD style);
	bool removeStyle(DWORD style);
	bool addStyleEx(DWORD styleEx);
	bool removeStyleEx(DWORD styleEx);
	
	//Window Status Getter
	bool isCreated(){ return created; }
	bool isRunning(){ return running; }
	bool isActivated(){ return activated; }
	bool isFullscreen(){ return fullscreen; }
	bool isMaximized(){ return maximized; }
	bool isMinimized(){ return minimized; }
	bool isResizing(){ return resizing; }
	bool isMoving(){ return moving; }

	//Window Status Changer
	bool setFullscreen(bool mode);
	bool setFullscreen(bool mode, int width, int height);
	bool setscreen();
	bool minimizeWindow();
	bool maximizeWindow();
	bool restoreWindow();

private:
	//Window Essencials
	HINSTANCE	hInstance;
	HWND		hWnd;
	WindowData windowData;
	
	//Window Status
	bool created;
	bool running;
	bool activated;
	bool fullscreen;
	bool maximized;
	bool minimized;
	bool resizing;
	bool moving;

};

COMA_END

