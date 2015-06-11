#include "stdafx.h"
#include "WindowEvent.h"
using namespace coma2d;

const std::string WindowEvent::CREATED			= "window_created";
const std::string WindowEvent::DESTROY			= "window_destroy";

const std::string WindowEvent::ACTIVATED		= "window_activated";
const std::string WindowEvent::INACTIVATED		= "window_inactivated";

const std::string WindowEvent::MINIMIZED		= "window_minimized";
const std::string WindowEvent::MAXIMIZED		= "window_maximized";
const std::string WindowEvent::RESTORED			= "window_restored";
const std::string WindowEvent::ENTER_FULLSCREEN	= "window_enterFullscreen";
const std::string WindowEvent::EXIT_FULLSCREEN	= "window_exitFullscreen";

const std::string WindowEvent::ENTER_RESIZEMOVE	= "window_enterResizeMove";
const std::string WindowEvent::EXIT_RESIZEMOVE	= "window_exitResizeMove";
const std::string WindowEvent::RESIZE			= "window_resize";
const std::string WindowEvent::RESIZING			= "window_resizing";
const std::string WindowEvent::MOVE				= "window_move";
const std::string WindowEvent::MOVING			= "window_moving";

const std::string WindowEvent::UPDATE			= "window_update";

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
