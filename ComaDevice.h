#pragma once
#include "ComaWindow.h"
#include "ComaRenderer.h"
namespace Coma2D
{
	class ComaDevice
	{
	public:
		ComaDevice();
		virtual ~ComaDevice();
		//Device Setter
		bool setWindow(ComaWindow* window);
		bool setRenderer(ComaRenderer* renderer);
		bool setWindowRenderer(ComaWindow* window, ComaRenderer* renderer);
		bool initDevice();
		bool run();

		//Device Getter
		ComaWindow* getWindow(){ return comaWindow; }
		ComaRenderer* getRenderer(){ return comaRenderer; }

		//Status Getter
		bool isRunning(){ return running; }
		bool isInitialized(){ return initialized; }

	public:
		void updateWindowListener(Event* event);
		void windowEnterResizeListener(Event* event);
		void windowExitResizeListener(Event* event);
		void windowMinimizeListener(Event* event);
		void windowRestoreListener(Event* event);
		void windowMaximizeListener(Event* event);
	private:
		//Device Essencials
		ComaWindow* comaWindow;
		ComaRenderer* comaRenderer;

		//DeviceStatus
		bool running;
		bool initialized;
	};
}