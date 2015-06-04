#pragma once
#include "Event.h"
#include "ComaWindow.h"
namespace Coma2D
{
	class WindowEvent :
		public Event
	{
	public:
		WindowEvent(std::string type, ComaWindow* target);
		~WindowEvent();
		static const std::string CREATED;
		static const std::string DESTROYED;
		static const std::string ACTIVATED;
		static const std::string INACTIVATED;
		static const std::string UPDATE;

		static const std::string RESIZE;
		static const std::string RESIZING;
		static const std::string ENTER_RESIZE;
		static const std::string EXIT_RESIZE;
		static const std::string ENTER_FULLSCREEN;
		static const std::string EXIT_FULLSCREEN;
		static const std::string MINIMIZED;
		static const std::string MAXIMIZED;
		static const std::string RESTORED;
		static const std::string MOVING;
	public:
		RECT	getWindowRect();
		RECT	getScreenSize();
		POINT	getWindowPosition();

		bool isActivated();
		bool isMinimized();
		bool isMaximized();
		bool isResizing();
		bool isRunning();
		bool isFullScreen();

		int getX();
		int getY();
		int getWidth();
		int getHeight();

		ComaWindow* getWindow();

	private:
		ComaWindow* target;
	};
}
