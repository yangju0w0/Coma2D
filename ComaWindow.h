#pragma once
#include "EventDispatcher.h"
#include "ComaRenderer.h"
#include <string>
namespace Coma2D
{
	class ComaWindow :
		public EventDispatcher
	{
	public:
		ComaWindow(HINSTANCE hInstance);
		virtual ~ComaWindow();

		//Window Running Func
		bool	createWindow();
		bool	run();
		void	close();
		LRESULT messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
	public:
		bool	updateRectData();	//windowRect, windowPosition 갱신

		//Window Size Getter
		RECT	getWindowRect();
		POINT	getWindowPosition();
		RECT	getScreenSize();

		//Window Size Setter
		bool setWindowPosition	(int x, int y);
		bool setWindowPosition	(POINT position);
		bool setScreenSize		(int width, int height);
		bool setScreenSize		(RECT size);
		bool setMinScreenSize	(int width, int height);
		bool setMinScreenSize	(RECT size);
		bool setMaxScreenSize	(int width, int height);
		bool setMaxScreenSize	(RECT size);
		bool setWindowRect		(int x, int y, int width, int height);
		bool setWindowRect		(RECT rect);

		//Window Status Getter
		bool isActivated(){ return activated; }
		bool isMinimized(){ return minimized; }
		bool isMaximized(){ return maximized; }
		bool isResizing(){ return resizing; }
		bool isRunning(){ return running; }
		bool isFullScreen(){ return fullscreen; }

		//Window Status Changer
		void minimizeWindow();
		void maximizeWindow();
		void restoreWindow();
		bool setFullScreen(bool mode=true, int width=800, int height=600);
		void showCursor(){ShowCursor(TRUE);}
		void hideCursor(){ ShowCursor(FALSE); }

		//Window Status Setter
		bool setIcon(HICON hIcon);
		bool setCursor(HCURSOR hCursor);
		bool setStyle(DWORD dwStyle);
		bool setStyleEx(DWORD dwStyleEx);
		bool removeStyle(DWORD dwStyle);
		bool removeStyleEx(DWORD dwStyleEx);
		bool setTitle(const char* name);
		bool setTitle(std::string name);

	private:
		bool changeWindowSize(POINT position, RECT size);	//윈도우 크기,위치 변경 후 각종 정보 갱신
	private:
		//Window Essencials
		HINSTANCE	hInstance;
		HWND		hWnd;

		//Window Attributes
		POINT	windowPosition;
		RECT	screenRect;
		RECT	minScreenRect;
		RECT	maxScreenRect;
		RECT	windowRect;
		POINT	fullscreenSize;
		HICON	hIcon;
		HCURSOR hCursor;
		DWORD	dwStyle;
		DWORD	dwStyleEx;
		const char* windowTitle;
		
		//Window Status
		bool activated;
		bool minimized;
		bool maximized;
		bool fullscreen;
		bool resizing;
		bool running;
	};
}
