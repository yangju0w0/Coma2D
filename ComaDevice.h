#pragma once
#include "ComaWindow.h"
#include "ComaRenderer.h"
namespace coma2d
{
	class ComaDevice
	{
	public:
		ComaDevice();
		ComaDevice(ComaWindow* window, ComaRenderer* renderer);
		virtual ~ComaDevice();

		//Device Setter
		bool setWindow(ComaWindow* window);
		bool setRenderer(ComaRenderer* renderer);
		bool setWindowRenderer(ComaWindow* window, ComaRenderer* renderer);

		//Device Control
		bool initDevice();
		bool initDevice(HINSTANCE hInstance);
		bool run();

		//Device Essencials Getter
		ComaWindow* getWindow(){ return window; }
		ComaRenderer* getRenderer(){ return renderer; }

		//Device Status Getter
		bool isRunning(){ return running; }
		bool isInitialized(){ return initialized; }

		void windowDestroyListener(Event* event);
		void windowMinimizeListener(Event* event);
		void windowRestoreListener(Event* event);
		void windowResizeListener(Event* event);
		void windowEnterResizeMoveListener(Event* event);
		void windowExitResizeMoveListener(Event* event);
		void windowUpdateListener(Event* event);
		void rendererUpdateListener(Event* event);
		void rendererRenderListener(Event* event);

	private:
		bool initWindow();
		bool initWindow(HINSTANCE hInstance);
		bool initRenderer();
	private:
		//Device Essencials
		ComaWindow* window;
		ComaRenderer* renderer;

		//Device Status
		bool running;
		bool initialized;
	};
}