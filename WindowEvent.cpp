#include "stdafx.h"
#include "WindowEvent.h"
using namespace Coma2D;

const std::string WindowEvent::CREATED			= "created";
const std::string WindowEvent::DESTROYED		= "destroyed";
const std::string WindowEvent::ACTIVATED		= "activated";
const std::string WindowEvent::INACTIVATED		= "inactivated";
const std::string WindowEvent::UPDATE			= "update";

const std::string WindowEvent::RESIZING			= "resizing";
const std::string WindowEvent::RESIZE			= "resize";
const std::string WindowEvent::MINIMIZED		= "minimized";
const std::string WindowEvent::MAXIMIZED		= "maximized";
const std::string WindowEvent::RESTORED			= "restored";
const std::string WindowEvent::ENTER_RESIZE		= "enterResize";
const std::string WindowEvent::EXIT_RESIZE		= "exitResize";
const std::string WindowEvent::ENTER_FULLSCREEN	= "enterFullscreen";
const std::string WindowEvent::EXIT_FULLSCREEN	= "exitFullscreen";
const std::string WindowEvent::MOVING			= "moving";

WindowEvent::WindowEvent(std::string type, ComaWindow* target)
	:Event(type)
{
	this->target = target;
}


WindowEvent::~WindowEvent()
{
}

RECT WindowEvent::getWindowRect()
{
	return target->getWindowRect();
}
RECT WindowEvent::getScreenSize()
{
	return target->getScreenSize();
}
POINT WindowEvent::getWindowPosition()
{
	return target->getWindowPosition();
}

bool WindowEvent::isActivated()
{
	return target->isActivated();
}
bool WindowEvent::isMinimized()
{
	return target->isMinimized();
}
bool WindowEvent::isMaximized()
{
	return target->isMaximized();
}
bool WindowEvent::isResizing()
{
	return target->isResizing();
}
bool WindowEvent::isRunning()
{
	return target->isRunning();
}
bool WindowEvent::isFullScreen()
{
	return target->isFullScreen();
}

ComaWindow* WindowEvent::getWindow()
{
	return target;
}

int WindowEvent::getX()
{
	return getWindowPosition().x;
}
int WindowEvent::getY()
{
	return getWindowPosition().y;
}
int WindowEvent::getWidth()
{
	return getScreenSize().right - getScreenSize().left;
}
int WindowEvent::getHeight()
{
	return getScreenSize().bottom - getScreenSize().top;
}