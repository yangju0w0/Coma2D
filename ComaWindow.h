#pragma once
#include "EventDispatcher.h"
#include "ComaRenderer.h"
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
		bool setWindowRect		(int x, int y, int width, int height);
		bool setWindowRect		(RECT rect);

		//Window Status Getter
		bool isActivated(){ return activated; }
		bool isMinimized(){ return minimized; }
		bool isMaximized(){ return maximized; }
		bool isResizing(){ return resizing; }
		bool isRunning(){ return running; }

	private:
		bool changeWindowSize(POINT position, RECT size);	//윈도우 크기,위치 변경 후 각종 정보 갱신
	private:
		//Window Essencials
		HINSTANCE	hInstance;
		HWND		hWnd;

		//Window Attributes
		POINT	windowPosition;
		RECT	screenRect;
		RECT	windowRect;
		
		//Window Status
		bool activated;
		bool minimized;
		bool maximized;
		bool resizing;
		bool running;

		/*
		//Getter
		HINSTANCE		getInstance()	{return hInstance;}
		HWND			getWindow()		{return hWnd;}
		ComaRenderer*	getRenderer()	{return renderer;}
		bool			isRunning()		{return isRunning;}


		//Setter
		bool			setRenderer(ComaRenderer* renderer);
		bool			setSize(int width, int height);
		bool			setMinimumSize(int width, int height);
		bool			setMaximumSize(int width, int height);
		void			setTitle(const char* title);
		

	private:
		//Windows Essensials
		HINSTANCE		hInstance;
		HWND			hWnd;
		ComaRenderer*	renderer;
		
		
		//Windows Attributes
		POINT			clientPosition;
		RECT			clientRect;
		RECT			minimumClientSize;
		RECT			maximumClientSize;
		bool			resizable;
		const char*		title;

		//Windows Status
		
		bool			isAvailable;
		bool			isResizing;
		bool			isRunning; */
	};
}

