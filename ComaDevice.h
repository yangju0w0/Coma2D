#pragma once
#include "ComaWindow.h"
#include "ComaRenderer.h"
#include "ResourceManager.h"
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
		ComaRenderer* getRenderer(){ return renderer; }

		//Device Control
		bool initDevice(HINSTANCE hInstance = nullptr);
		bool run();

		//Device Status Getter
		bool isRunning(){ return running; }
		bool isInitialized(){ return initialized; }

		//Manager Getter
		ResourceManager* getResourceManager(){ return resourceManager; }


		//Events
		void _windowDestroyListener(Event* event);
		void _windowMinimizeListener(Event* event);
		void _windowRestoreListener(Event* event);
		void _windowResizeListener(Event* event);
		void _windowEnterResizeMoveListener(Event* event);
		void _windowExitResizeMoveListener(Event* event);
		void _windowUpdateListener(Event* event);
		void _rendererUpdateListener(Event* event);
		void _rendererRenderListener(Event* event);

	private:
		bool initWindow();
		bool initWindow(HINSTANCE hInstance);
		bool initRenderer();
		bool initManagers();
	private:
		//Device Essencials
		ComaWindow* window;
		ComaRenderer* renderer;

		//Device Status
		bool running;
		bool initialized;

		//Managers
		ResourceManager* resourceManager;
	};
}