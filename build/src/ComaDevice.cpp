/*
* The MIT License (MIT)
*
* Copyright (c) 2015 Heonyang Ju
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
* MIT 라이선스 (MIT)
*
* 저작권(C) 2015 주헌양
*
* 이 문서에 의해, 아래의 조건을 따르는 한 제한 없이 소프트웨어를 다루기 위해
* 소프트웨어의 복사본과 관련된 문서("소프트웨어")를 획득하는 사람에게 변경의
* 자유가 허용 된다. 제한 없음은 사용, 복제, 수정, 통합, 공포, 배포, 하위 라이선스
* 그리고/또는 소프트웨어의 복사본의 판매와 공급된 소프트웨어를 가진 사람에게
* 그렇게 할 수 있도록 허가하는 권한에 제한 없음을 포함 한다.
*
* 위의 저작권 표시와 이 허가 표시는 모든 소프트웨어의 복사본 또는 상당한 일부분에
* 포함되어야 한다.
*
* 이 소프트웨어는 특정한 목적과 침해 없음을 위해 상업성, 적합성의 보증에만
* 국한되는 것이 아닌 명시적 또는 묵시적인 어떠한 종류의 보증없이 "있는 그대로"
* 제공 된다. 어떠한 경우에도 저작자 또는 저작권자는 소프트웨어의 관계 안과 밖
* 또는 소프트웨어의 사용 또는 거래로부터 발생하는 계약, 불법 행위 또는 기타 수행에
* 대해, 어떠한 소송, 손해 또는 다른 책무에 대해서도 책임이 없다.
*
* 본문은 MIT라이선스 원문의 번역본이며, 모든 해석은 원문에 기초한다.
*/
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
bool ComaDevice::initManagers()
{
	resourceManager = new ResourceManager();
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

}
void ComaDevice::rendererRenderListener(Event* event)
{

}