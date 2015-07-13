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
	bool	createWindow();
	bool	run();

	LRESULT messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	HINSTANCE	getInstance() { return hInstance; }
	HWND		getWindow(){ return hWnd; }
	InputManager* getInputManager(){ return inputManager; }

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

	//Manager
	InputManager* inputManager;
	
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

