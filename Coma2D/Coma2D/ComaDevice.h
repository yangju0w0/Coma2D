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
	ComaDevice();
	ComaDevice(const ComaDevice& other);
	~ComaDevice();

	static ComaDevice* device;

public:
	static ComaDevice* GetDevice();

	void Release();
	void ReleaseAll();

public:
	//Device Setter
	bool SetWindow(ComaWindow* window);
	bool SetRenderer(ComaRenderer* renderer);
	bool SetWindowRenderer(ComaWindow* window, ComaRenderer* renderer);

	//Device Getter
	ComaWindow* GetWindow() const { return window_; }
	ComaRenderer* GetRenderer() const { return renderer_; }

	//Device Control
	bool InitDevice();
	bool InitDevice(HINSTANCE hInstance);
	bool Run();

	//Device Status Getter
	bool IsRunning() const { return running_; }
	bool IsInitialized() const { return initialized_; }

	//ManagerGetter
	ResourceManager* GetResourceManager() const { return resourceManager_; }
	SceneManager* GetSceneManager() const { return sceneManager_; }
	InputManager* GetInputManager() const { return inputManager_; }
	PhysicsManager* GetPhysicsManager() const { return physicsManager_; }

private:
	//Events
	void WindowDestroyListener(const Event* event);
	void WindowMinimizeListener(const Event* event);
	void WindowRestoreListener(const Event* event);
	void WindowResizeListener(const Event* event);
	void WindowEnterResizeMoveListener(const Event* event);
	void WindowExitResizeMoveListener(const Event* event);
	void WindowUpdateListener(const Event* event);
	void RendererUpdateListener(const Event* event);
	void RendererRenderListener(const Event* event);

private:
	bool InitWindow();
	bool InitWindow(HINSTANCE hInstance);
	bool InitRenderer();
	bool InitManagers();
	bool InitOthers();
private:
	//Device Essencials
	ComaWindow* window_;
	ComaRenderer* renderer_;

	//Device Status
	bool running_;
	bool initialized_;

	//Managers
	ResourceManager* resourceManager_;
	SceneManager* sceneManager_;
	InputManager* inputManager_;
	PhysicsManager* physicsManager_;
};

COMA_END