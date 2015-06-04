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

	comaWindow->setEventListener(WindowEvent::UPDATE, BIND(ComaDevice::updateWindowListener));

	if (!comaWindow->isRunning())
		comaWindow->run();

	running = true;
	return true;
}

void ComaDevice::updateWindowListener(Event* event)
{
	comaRenderer->render();
}