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
* filename	WindowEvent.h
* fileinfo	윈도우 생성 클래스의 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Event.h"
#include "ComaWindow.h"

COMA_NS_BEGIN

class WindowEvent :
	public Event
{
public:
	WindowEvent(std::string type, ComaWindow* target, WPARAM wParam = NULL, LPARAM lParam = NULL);
	~WindowEvent();

public:
	static const std::string CREATED;
	static const std::string DESTROY;

	static const std::string ACTIVATED;
	static const std::string INACTIVATED;

	static const std::string MINIMIZED;
	static const std::string MAXIMIZED;
	static const std::string RESTORED;

	static const std::string RESIZE;
	static const std::string RESIZING;
	static const std::string MOVE;
	static const std::string MOVING;

	static const std::string ENTER_FULLSCREEN;
	static const std::string EXIT_FULLSCREEN;

	static const std::string ENTER_RESIZEMOVE;
	static const std::string EXIT_RESIZEMOVE;
	
	
	static const std::string UPDATE;

public:
	POINT getWindowPosition(){ return target->getWindowPosition(); }
	RECT getScreenSize(){ return target->getScreenSize(); }
	RECT getWindowSize(){ return target->getWindowSize(); }
	RECT getWindowRect(){ return target->getWindowRect(); }

	int getWindowX(){ return target->getWindowPosition().x; }
	int getWindowY(){ return target->getWindowPosition().y; }
	int getScreenWidth(){ return target->getScreenSize().right; }
	int getScreenHeight(){ return target->getScreenSize().bottom; }
	int getWindowWidth(){ return target->getWindowSize().right; }
	int getWindowHeight(){ return target->getWindowSize().bottom; }
	
	bool isCreated(){ return target->isCreated(); }
	bool isRunning(){ return target->isRunning(); }
	bool isActivated(){ return target->isActivated(); }
	bool isFullscreen(){ return target->isFullscreen(); }
	bool isMaximized(){ return target->isMaximized(); }
	bool isMinimized(){ return target->isMinimized(); }
	bool isResizing(){ return target->isResizing(); }
	bool isMoving(){ return target->isMoving(); }

	ComaWindow* getTarget(){ return target; }
	WPARAM getWParam(){ return wParam; }
	LPARAM getLParam(){ return lParam; }
private:
	ComaWindow* target;
	WPARAM wParam;
	LPARAM lParam;
};
COMA_END