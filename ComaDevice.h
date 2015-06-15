#pragma once
#include "ComaWindow.h"
#include "ComaRenderer.h"
namespace coma2d
{
	class ComaDevice
	{
	private:
		ComaDevice(){};
		ComaDevice(const ComaDevice& other);
		~ComaDevice(){};
		static ComaDevice* device;
	public:
		static ComaDevice* GetDevice()
		{
			if (device == 0) device = new ComaDevice();
			return device;
		}
	public:
		//Device Setter
		bool setWindow(ComaWindow* window);
		bool setRenderer(ComaRenderer* renderer);
		bool setWindowRenderer(ComaWindow* window, ComaRenderer* renderer);

		//Device Getter
		ComaWindow* getWindow(){ return window; }
		ComaRenderer* getRemderer(){ return renderer; }

		//Device Control
		bool initDevice();
		bool initDevice(HINSTANCE hInstance);
		bool run();

		//Device Status Getter
		bool isRunning(){ return running; }
		bool isInitialized(){ return initialized; }


		//Events
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