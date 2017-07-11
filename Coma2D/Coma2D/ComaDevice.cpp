/* 
* filename	ComaDevice.cpp
* fileinfo	프레임워크 관리 싱글턴 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "ComaDevice.h"

#include "Bitmap.h"

COMA_USING_NS

ComaDevice* ComaDevice::device;

bool ComaDevice::setWindow(ComaWindow* window)
{
	if (initialized)
		return false;
	this->window = window;
	return true;
}
bool ComaDevice::setRenderer(ComaRenderer* renderer)
{
	if (initialized)
		return false;
	this->renderer = renderer;
	return true;
}
bool ComaDevice::setWindowRenderer(ComaWindow* window, ComaRenderer* renderer)
{
	if (!setWindow(window))
		return false;
	if (!setRenderer(renderer))
		return false;
	return true;
}

bool ComaDevice::initDevice()
{
	if (initialized)
		return false;
	if (!initWindow())
		return false;
	if (!initRenderer())
		return false;
	if (!initManagers())
		return false;
	if (!initOthers())
		return false;
	initialized = true;
	return true;
}
bool ComaDevice::initDevice(HINSTANCE hInstance)
{
	if (initialized)
		return false;
	if (!initWindow(hInstance))
		return false;
	if (!initRenderer())
		return false;
	if (!initManagers())
		return false;
	if (!initOthers())
		return false;
	initialized = true;
	return true;
}

bool ComaDevice::run()
{
	if (running || !initialized || !window || !renderer)
		return false;
	running = true;

	if (!renderer->isRunning())
		renderer->run();

	if (!window->isRunning())
		window->run();

	return true;
}

bool ComaDevice::initWindow()
{
	if (!window)
		return false;
	if (!window->isCreated())
		if (!window->createWindow())
			return false;

	window->SetEventListener(WindowEvent::DESTROY, BIND(ComaDevice::windowDestroyListener), this);
	window->SetEventListener(WindowEvent::MINIMIZED, BIND(ComaDevice::windowMinimizeListener), this);
	window->SetEventListener(WindowEvent::RESTORED, BIND(ComaDevice::windowRestoreListener), this);
	window->SetEventListener(WindowEvent::RESIZE, BIND(ComaDevice::windowResizeListener), this);
	window->SetEventListener(WindowEvent::ENTER_RESIZEMOVE, BIND(ComaDevice::windowEnterResizeMoveListener), this);
	window->SetEventListener(WindowEvent::EXIT_RESIZEMOVE, BIND(ComaDevice::windowExitResizeMoveListener), this);
	window->SetEventListener(WindowEvent::UPDATE, BIND(ComaDevice::windowUpdateListener), this);
	return true;
}
bool ComaDevice::initWindow(HINSTANCE hInstance)
{
	if (window)
		return initWindow();

	window = new ComaWindow(hInstance);
	return initWindow();
}

bool ComaDevice::initRenderer()
{
	if (!window)
		return false;

	if (!renderer)
		renderer = new ComaRenderer();

	if (!renderer->isInitialized())
		if (!renderer->initRenderer(window->getWindow()))
			return false;

	renderer->SetEventListener(RendererEvent::UPDATE, BIND(ComaDevice::rendererUpdateListener), this);
	renderer->SetEventListener(RendererEvent::RENDER, BIND(ComaDevice::rendererRenderListener), this);
		
	return true;
}
bool ComaDevice::initManagers()
{
	resourceManager = new ResourceManager();
	sceneManager = new SceneManager();
	inputManager = window->getInputManager();
	physicsManager = new PhysicsManager();
	physicsManager->initBox2D(Vector{ 0.0f, 10.0f });
	return true;
}
bool ComaDevice::initOthers()
{
	Bitmap::setRenderer(renderer);
	return true;
}

void ComaDevice::windowDestroyListener(Event* event)
{
	
}
void ComaDevice::windowMinimizeListener(Event* event)
{
	renderer->pause();
}
void ComaDevice::windowRestoreListener(Event* event)
{
	renderer->run();
}
void ComaDevice::windowEnterResizeMoveListener(Event* event)
{
	renderer->pause();
}
void ComaDevice::windowExitResizeMoveListener(Event* event)
{
	renderer->resetSize();
	renderer->run();
}
void ComaDevice::windowUpdateListener(Event* event)
{
	renderer->update();
}
void ComaDevice::windowResizeListener(Event* event)
{
	if (!window->isResizing() && !window->isMinimized())
		renderer->resetSize();
}

void ComaDevice::rendererUpdateListener(Event* event)
{
	RendererEvent* rEvent = (RendererEvent*)event;

	inputManager->updateGamepad(rEvent->DeltaTime());

	physicsManager->nextStep(rEvent->DeltaTime());
	if (!sceneManager || !sceneManager->getScene())
		return;
	sceneManager->getScene()->update(rEvent->DeltaTime());
}
void ComaDevice::rendererRenderListener(Event* event)
{
	RendererEvent* rEvent = (RendererEvent*)event;

	if (!sceneManager || !sceneManager->getScene())
		return;
	sceneManager->getScene()->render(rEvent->GetRenderTarget(), rEvent->DeltaTime());
}