#include "stdafx.h"
#include "ComaDevice.h"
#include "WindowEvent.h"
using namespace Coma2D;

ComaDevice::ComaDevice()
	:running(false), initialized(false)
{
}


ComaDevice::~ComaDevice()
{
}

bool ComaDevice::setWindow(ComaWindow* window)
{
	if (running)
		return false;
	comaWindow = window;
	return true;
}

bool ComaDevice::setRenderer(ComaRenderer* renderer)
{
	if (running)
		return false;
	comaRenderer = renderer;
	return true;

}

bool ComaDevice::setWindowRenderer(ComaWindow* window, ComaRenderer* renderer)
{
	if (running)
		return false;
	comaWindow = window;
	comaRenderer = renderer;
	return true;
}
bool ComaDevice::initDevice()
{
	if (initialized || running || !comaWindow || !comaRenderer)
		return false;
	if (!comaWindow->isCreated())
		if (!comaWindow->createWindow())
			return false;
	if (!comaRenderer->isInitialized())
		if (!comaRenderer->initRenderer(comaWindow->getWindow()))
			return false;
	initialized = true;
	return true;
}
bool ComaDevice::run()
{
	if (running || !comaWindow || !comaRenderer)
		return false;
	if (!comaWindow->isCreated() || !comaRenderer->isInitialized())
		return false;
	if (!comaRenderer->isInitialized())
		return false;

	if (!comaRenderer->isRunning())
		comaRenderer->run();

	comaWindow->setEventListener(WindowEvent::UPDATE, BIND(ComaDevice::updateWindowListener));
	comaWindow->setEventListener(WindowEvent::EXIT_RESIZE, BIND(ComaDevice::windowExitResizeListener));
	comaWindow->setEventListener(WindowEvent::ENTER_RESIZE, BIND(ComaDevice::windowEnterResizeListener));
	comaWindow->setEventListener(WindowEvent::MINIMIZED, BIND(ComaDevice::windowMinimizeListener));
	comaWindow->setEventListener(WindowEvent::MAXIMIZED, BIND(ComaDevice::windowMaximizeListener));
	comaWindow->setEventListener(WindowEvent::RESTORED, BIND(ComaDevice::windowRestoreListener));


	running = true;
	if (!comaWindow->isRunning())
		comaWindow->run();

	return true;
}

void ComaDevice::updateWindowListener(Event* event)
{
	if (!comaRenderer->isRunning())
	{
		Sleep(100);
		return;
	}
	else
	{
	
	}
	comaRenderer->render();
	comaWindow->setTitle(std::to_string(comaRenderer->getFPS()));
}
void ComaDevice::windowEnterResizeListener(Event* event)
{
	comaRenderer->pause();	
}
void ComaDevice::windowExitResizeListener(Event* event)
{
	comaRenderer->resizeWindow();
	comaRenderer->run();
}
void ComaDevice::windowMinimizeListener(Event* event)
{
	//comaRenderer->pause();
}
void ComaDevice::windowRestoreListener(Event* event)
{
	//comaRenderer->resizeWindow();
	//comaRenderer->run();
}
void ComaDevice::windowMaximizeListener(Event* event)
{
	//comaRenderer->resizeWindow();
	//comaRenderer->run();
}