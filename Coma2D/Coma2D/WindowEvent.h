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
	WindowEvent(const std::string& type, ComaWindow* target, WPARAM wParam = NULL, LPARAM lParam = NULL);
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
	POINT GetWindowPosition() const { return target_->GetWindowPosition(); }
	RECT GetScreenSize() const { return target_->GetScreenSize(); }
	RECT GetWindowSize() const { return target_->GetWindowSize(); }
	RECT GetWindowRect() const { return target_->GetWindowRect(); }

	int GetWindowX() const { return target_->GetWindowPosition().x; }
	int GetWindowY() const { return target_->GetWindowPosition().y; }
	int GetScreenWidth() const { return target_->GetScreenSize().right; }
	int GetScreenHeight() const { return target_->GetScreenSize().bottom; }
	int GetWindowWidth() const { return target_->GetWindowSize().right; }
	int GetWindowHeight() const { return target_->GetWindowSize().bottom; }

	bool IsCreated() const { return target_->IsCreated(); }
	bool IsRunning() const { return target_->IsRunning(); }
	bool IsActivated() const { return target_->IsActivated(); }
	bool IsFullscreen() const { return target_->IsFullscreen(); }
	bool IsMaximized() const { return target_->IsMaximized(); }
	bool IsMinimized() const { return target_->IsMinimized(); }
	bool IsResizing() const { return target_->IsResizing(); }
	bool IsMoving() const { return target_->IsMoving(); }

	ComaWindow* GetTarget() const { return target_; }
	WPARAM GetWParam() const { return wParam_; }
	LPARAM GetLParam() const { return lParam_; }
private:
	ComaWindow* target_;
	WPARAM wParam_;
	LPARAM lParam_;
};
COMA_END