#include "stdafx.h"
#include "ComaWindow.h"
using namespace coma2d;

ComaWindow* comaWindow;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return comaWindow->messageProc(hWnd, uMsg, wParam, lParam);
}

ComaWindow::ComaWindow(HINSTANCE hInstance)
	:hInstance(0), hWnd(0)
{
	this->hInstance = hInstance;
	comaWindow = this;
}


ComaWindow::~ComaWindow()
{
}


bool ComaWindow::createWindow()
{
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

	RECT rect = { 0, 0, 800, 480 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	
	hWnd = CreateWindow(
		className,
		"Coma2DWindow",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		rect.right - rect.left, rect.bottom - rect.top,
		0, 0, hInstance, 0);
	
	if (!hWnd)
		return false;
	
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	
	return true;
}

bool ComaWindow::run()
{
	if (!hWnd)
		return false;
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
			
		}
	}
	return true;
}

LRESULT ComaWindow::messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}