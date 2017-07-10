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