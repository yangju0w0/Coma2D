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

		bool	createWindow();
		bool	run();
		
		LRESULT messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE	hInstance;
		HWND		hWnd;
		

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

