#include "stdafx.h"
#include "ComaDevice.h"
#include "WindowEvent.h"
#include "RendererEvent.h"

using namespace coma2d;

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


bool ComaDevice::initDevice(HINSTANCE hInstance)
{
	if (initialized)
		return false;
	if (!hInstance)
	{
		if (!initWindow())
			return false;
	}
	else
	{
		if (!initWindow(hInstance))
			return false;
	}
	if (!initRenderer())
		return false;
	if (!initManagers())
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

	window->setEventListener(WindowEvent::DESTROY,			BIND(ComaDevice::_windowDestroyListener			));
	window->setEventListener(WindowEvent::MINIMIZED,		BIND(ComaDevice::_windowMinimizeListener			));
	window->setEventListener(WindowEvent::RESTORED,			BIND(ComaDevice::_windowRestoreListener			));
	window->setEventListener(WindowEvent::RESIZE,			BIND(ComaDevice::_windowResizeListener			));
	window->setEventListener(WindowEvent::ENTER_RESIZEMOVE, BIND(ComaDevice::_windowEnterResizeMoveListener	));
	window->setEventListener(WindowEvent::EXIT_RESIZEMOVE,	BIND(ComaDevice::_windowExitResizeMoveListener	));
	window->setEventListener(WindowEvent::UPDATE,			BIND(ComaDevice::_windowUpdateListener			));
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

	renderer->setEventListener(RendererEvent::UPDATE, BIND(ComaDevice::_rendererUpdateListener));
	renderer->setEventListener(RendererEvent::RENDER, BIND(ComaDevice::_rendererRenderListener));
		
	return true;
}

bool ComaDevice::initManagers()
{
	resourceManager = new ResourceManager(renderer);
	return true;
}

void ComaDevice::_windowDestroyListener(Event* event)
{
	
}
void ComaDevice::_windowMinimizeListener(Event* event)
{
	renderer->pause();
}
void ComaDevice::_windowRestoreListener(Event* event)
{
	renderer->run();
}
void ComaDevice::_windowEnterResizeMoveListener(Event* event)
{
	renderer->pause();
}
void ComaDevice::_windowExitResizeMoveListener(Event* event)
{
	renderer->resetSize();
	renderer->run();
}
void ComaDevice::_windowUpdateListener(Event* event)
{
	renderer->update();
}
void ComaDevice::_windowResizeListener(Event* event)
{
	if (!window->isResizing() && !window->isMinimized())
		renderer->resetSize();
}

void ComaDevice::_rendererUpdateListener(Event* event)
{

}
void ComaDevice::_rendererRenderListener(Event* event)
{

}