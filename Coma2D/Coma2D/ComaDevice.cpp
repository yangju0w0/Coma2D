/* 
* filename	ComaDevice.cpp
* fileinfo	프레임워크 관리 싱글턴 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "ComaDevice.h"

#include "Bitmap.h"

COMA_USING_NS

ComaDevice* ComaDevice::device;

ComaDevice::ComaDevice() :window_(nullptr), renderer_(nullptr), running_(false), initialized_(false), resourceManager_(nullptr), sceneManager_(nullptr), inputManager_(nullptr), physicsManager_(nullptr)
{}

ComaDevice::~ComaDevice()
{
	if (resourceManager_) delete resourceManager_;
	if (sceneManager_) delete sceneManager_;
	if (physicsManager_) delete physicsManager_;
	//inputManager은 window에서 가져오므로 해제할 필요가 없다.
}

ComaDevice* ComaDevice::GetDevice()
{
	if (device == 0)
	{
		device = new ComaDevice();
	}
	return device;
}

void ComaDevice::Release()
{
	if (initialized_)
	{
		if (window_)
		{
			window_->RemoveEventListener(WindowEvent::DESTROY, this);
			window_->RemoveEventListener(WindowEvent::MINIMIZED, this);
			window_->RemoveEventListener(WindowEvent::RESTORED, this);
			window_->RemoveEventListener(WindowEvent::RESIZE, this);
			window_->RemoveEventListener(WindowEvent::ENTER_RESIZEMOVE, this);
			window_->RemoveEventListener(WindowEvent::EXIT_RESIZEMOVE, this);
			window_->RemoveEventListener(WindowEvent::UPDATE, this);
		}
		if (renderer_)
		{
			renderer_->RemoveEventListener(RendererEvent::UPDATE, this);
			renderer_->RemoveEventListener(RendererEvent::RENDER, this);
		}
	}
	delete device;
}

void ComaDevice::ReleaseAll()
{
	if (window_) delete window_;
	if (renderer_) delete renderer_;
	delete device;
}

bool ComaDevice::SetWindow(ComaWindow* window)
{
	if (initialized_)
	{
		return false;
	}
	this->window_ = window;
	return true;
}

bool ComaDevice::SetRenderer(ComaRenderer* renderer)
{
	if (initialized_)
	{
		return false;
	}
	this->renderer_ = renderer;
	return true;
}

bool ComaDevice::SetWindowRenderer(ComaWindow* window, ComaRenderer* renderer)
{
	if (!SetWindow(window))
	{
		return false;
	}

	if (!SetRenderer(renderer))
	{
		return false;
	}

	return true;
}

bool ComaDevice::InitDevice()
{
	if (initialized_)
	{
		return false;
	}
	if (!InitWindow())
	{
		return false;
	}
	if (!InitRenderer())
	{
		return false;
	}
	if (!InitManagers())
	{
		return false;
	}
	if (!InitOthers())
	{
		return false;
	}
	initialized_ = true;
	return true;
}

bool ComaDevice::InitDevice(HINSTANCE hInstance)
{
	if (initialized_)
	{
		return false;
	}
	if (!InitWindow(hInstance))
	{
		return false;
	}
	if (!InitRenderer())
	{
		return false;
	}
	if (!InitManagers())
	{
		return false;
	}
	if (!InitOthers())
	{
		return false;
	}
	initialized_ = true;
	return true;
}

bool ComaDevice::Run()
{
	if (running_ || !initialized_ || !window_ || !renderer_)
	{
		return false;
	}
	running_ = true;

	if (!renderer_->IsRunning())
	{
		renderer_->Run();
	}

	if (!window_->IsRunning())
	{
		window_->Run();
	}

	return true;
}

bool ComaDevice::InitWindow()
{
	if (!window_)
	{
		return false;
	}

	if (!window_->IsCreated())
	{
		if (!window_->CreateComaWindow())
		{
			return false;
		}
	}

	window_->SetEventListener(WindowEvent::DESTROY, BIND(ComaDevice::WindowDestroyListener), this);
	window_->SetEventListener(WindowEvent::MINIMIZED, BIND(ComaDevice::WindowMinimizeListener), this);
	window_->SetEventListener(WindowEvent::RESTORED, BIND(ComaDevice::WindowRestoreListener), this);
	window_->SetEventListener(WindowEvent::RESIZE, BIND(ComaDevice::WindowResizeListener), this);
	window_->SetEventListener(WindowEvent::ENTER_RESIZEMOVE, BIND(ComaDevice::WindowEnterResizeMoveListener), this);
	window_->SetEventListener(WindowEvent::EXIT_RESIZEMOVE, BIND(ComaDevice::WindowExitResizeMoveListener), this);
	window_->SetEventListener(WindowEvent::UPDATE, BIND(ComaDevice::WindowUpdateListener), this);

	return true;
}

bool ComaDevice::InitWindow(HINSTANCE hInstance)
{
	if (window_)
	{
		return InitWindow();
	}

	window_ = new ComaWindow(hInstance);
	return InitWindow();
}

bool ComaDevice::InitRenderer()
{
	if (!window_)
	{
		return false;
	}

	if (!renderer_)
	{
		renderer_ = new ComaRenderer();
	}

	if (!renderer_->IsInitialized())
	{
		if (!renderer_->InitRenderer(window_->GetWindow()))
		{
			return false;
		}
	}
	
	renderer_->SetEventListener(RendererEvent::UPDATE, BIND(ComaDevice::RendererUpdateListener), this);
	renderer_->SetEventListener(RendererEvent::RENDER, BIND(ComaDevice::RendererRenderListener), this);
		
	return true;
}

bool ComaDevice::InitManagers()
{
	resourceManager_ = new ResourceManager();
	sceneManager_ = new SceneManager();
	inputManager_ = window_->GetInputManager();
	physicsManager_ = new PhysicsManager();
	physicsManager_->initBox2D(Vector{ 0.0f, 10.0f });

	return true;
}

bool ComaDevice::InitOthers()
{
	Bitmap::setRenderer(renderer_);
	return true;
}

void ComaDevice::WindowDestroyListener(const Event* event)
{}

void ComaDevice::WindowMinimizeListener(const Event* event)
{
	renderer_->Pause();
}

void ComaDevice::WindowRestoreListener(const Event* event)
{
	renderer_->Run();
}

void ComaDevice::WindowEnterResizeMoveListener(const Event* event)
{
	renderer_->Pause();
}

void ComaDevice::WindowExitResizeMoveListener(const Event* event)
{
	renderer_->ResetSize();
	renderer_->Run();
}

void ComaDevice::WindowUpdateListener(const Event* event)
{
	renderer_->Update();
}

void ComaDevice::WindowResizeListener(const Event* event)
{
	if (!window_->IsResizing() && !window_->IsMinimized())
	{
		renderer_->ResetSize();
	}
}

void ComaDevice::RendererUpdateListener(const Event* event)
{
	RendererEvent* rEvent = (RendererEvent*)event;

	inputManager_->updateGamepad(rEvent->DeltaTime());

	physicsManager_->nextStep(rEvent->DeltaTime());

	Scene* scene = sceneManager_->getScene();
	if (scene)
	{
		scene->update(rEvent->DeltaTime());
	}
}

void ComaDevice::RendererRenderListener(const Event* event)
{
	RendererEvent* rEvent = (RendererEvent*)event;

	Scene* scene = sceneManager_->getScene();
	if (scene)
	{
		scene->render(rEvent->GetRenderTarget(), rEvent->DeltaTime());
	}
}