/*
* filename	ComaDevice.h
* fileinfo	프레임워크 관리 싱글턴 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "ComaWindow.h"
#include "ComaRenderer.h"
#include "comaUtil.h"
#include "WindowEvent.h"
#include "RendererEvent.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"

COMA_NS_BEGIN

class ComaDevice
{
private:
	ComaDevice() :window(nullptr), renderer(nullptr), running(false), initialized(false), resourceManager(nullptr), sceneManager(nullptr), inputManager(nullptr), physicsManager(nullptr) {};
	ComaDevice(const ComaDevice& other);
	~ComaDevice()
	{
		if(resourceManager) delete resourceManager;
		if(sceneManager) delete sceneManager;
		if(physicsManager) delete physicsManager;
		//inputManager은 window에서 가져오므로 해제할 필요가 없다.
		
	};
	static ComaDevice* device;
public:
	static ComaDevice* GetDevice()
	{
		if (device == 0) device = new ComaDevice();
		return device;
	}
	void Release(){ 
		if (initialized)
		{
			if (window)
			{
				window->RemoveEventListener(WindowEvent::DESTROY,			this);
				window->RemoveEventListener(WindowEvent::MINIMIZED,			this);
				window->RemoveEventListener(WindowEvent::RESTORED,			this);
				window->RemoveEventListener(WindowEvent::RESIZE,			this);
				window->RemoveEventListener(WindowEvent::ENTER_RESIZEMOVE,	this);
				window->RemoveEventListener(WindowEvent::EXIT_RESIZEMOVE,	this);
				window->RemoveEventListener(WindowEvent::UPDATE,			this);
			}
			if (renderer)
			{
				renderer->RemoveEventListener(RendererEvent::UPDATE, this);
				renderer->RemoveEventListener(RendererEvent::RENDER, this);
			}
		}
		delete device;
	}
	void ReleaseAll()
	{
		if (window) delete window;
		if (renderer) delete renderer;
		delete device;
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
	bool initDevice();
	bool initDevice(HINSTANCE hInstance);
	bool run();

	//Device Status Getter
	bool isRunning(){ return running; }
	bool isInitialized(){ return initialized; }

	//ManagerGetter
	ResourceManager* getResourceManager(){ return resourceManager; }
	SceneManager* getSceneManager(){ return sceneManager; }
	InputManager* getInputManager(){ return inputManager; }
	PhysicsManager* getPhysicsManager(){ return physicsManager; }

private:
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
	bool initManagers();
	bool initOthers();
private:
	//Device Essencials
	ComaWindow* window;
	ComaRenderer* renderer;

	//Device Status
	bool running;
	bool initialized;

	//Managers
	ResourceManager* resourceManager;
	SceneManager* sceneManager;
	InputManager* inputManager;
	PhysicsManager* physicsManager;
};

COMA_END