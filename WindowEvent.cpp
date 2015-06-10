#include "stdafx.h"
#include "WindowEvent.h"
using namespace coma2d;

const std::string WindowEvent::CREATED			= "created";
const std::string WindowEvent::DESTROY			= "destroy";

const std::string WindowEvent::ACTIVATED		= "activated";
const std::string WindowEvent::INACTIVATED		= "inactivated";

const std::string WindowEvent::MINIMIZED		= "minimized";
const std::string WindowEvent::MAXIMIZED		= "maximized";
const std::string WindowEvent::RESTORED			= "restored";
const std::string WindowEvent::ENTER_FULLSCREEN	= "enterFullscreen";
const std::string WindowEvent::EXIT_FULLSCREEN	= "exitFullscreen";

const std::string WindowEvent::ENTER_RESIZEMOVE	= "enterResizeMove";
const std::string WindowEvent::EXIT_RESIZEMOVE	= "exitResizeMove";
const std::string WindowEvent::RESIZE			= "resize";
const std::string WindowEvent::RESIZING			= "resizing";
const std::string WindowEvent::MOVE				= "move";
const std::string WindowEvent::MOVING			= "moving";

const std::string WindowEvent::UPDATE			= "update";

WindowEvent::WindowEvent(std::string type, ComaWindow* target, WPARAM wParam, LPARAM lParam)
	:Event(type)
{
	this->target = target;
	this->wParam = wParam;
	this->lParam = lParam;
}


WindowEvent::~WindowEvent()
{
}
