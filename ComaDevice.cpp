#include "stdafx.h"
#include "ComaDevice.h"
#include "WindowEvent.h"
#include "RendererEvent.h"

using namespace coma2d;

ComaDevice::ComaDevice()
	:running(false), initialized(false), window(nullptr), renderer(nullptr)
{
}

ComaDevice::ComaDevice(ComaWindow* window, ComaRenderer* renderer)
	: running(false), initialized(false)
{
	setWindowRenderer(window, renderer);
}

ComaDevice::~ComaDevice()
{
}

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

	window->setEventListener(WindowEvent::DESTROY,			BIND(ComaDevice::windowDestroyListener			));
	window->setEventListener(WindowEvent::MINIMIZED,		BIND(ComaDevice::windowMinimizeListener			));
	window->setEventListener(WindowEvent::RESTORED,			BIND(ComaDevice::windowRestoreListener			));
	window->setEventListener(WindowEvent::RESIZE,			BIND(ComaDevice::windowResizeListener			));
	window->setEventListener(WindowEvent::ENTER_RESIZEMOVE, BIND(ComaDevice::windowEnterResizeMoveListener	));
	window->setEventListener(WindowEvent::EXIT_RESIZEMOVE,	BIND(ComaDevice::windowExitResizeMoveListener	));
	window->setEventListener(WindowEvent::UPDATE,			BIND(ComaDevice::windowUpdateListener			));
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

	renderer->setEventListener(RendererEvent::UPDATE, BIND(ComaDevice::rendererUpdateListener));
	renderer->setEventListener(RendererEvent::RENDER, BIND(ComaDevice::rendererRenderListener));
		
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

}
void ComaDevice::rendererRenderListener(Event* event)
{

}