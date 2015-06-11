#include "stdafx.h"
#include "ComaRenderer.h"
using namespace coma2d;


ComaRenderer::ComaRenderer()
	:initialized(false), running(false), deltaTime(0.0), fps(0.0f), frameCount(0)
{
	timer = new RenderTimer();
	backgroundColor = D2D1::ColorF(0.0f, 0.0f, 0.0f);
	maxFrameTime = 0.05f;
	minFrameTime = 0.01f;
}


ComaRenderer::~ComaRenderer()
{
	if (factory)factory->Release();
	if (renderTarget)renderTarget->Release();
}

bool ComaRenderer::initRenderer(HWND hWnd)
{
	if (initialized)
		return false;

	if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory) != S_OK)
		return false;
	if (!createRenderTarget(hWnd))
		return false;

	timer->reset();

	targetWindow = hWnd;
	initialized = true;
	return true;
}

bool ComaRenderer::createRenderTarget(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	if (factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&renderTarget
		) != S_OK)
		return false;
	return true;
}
void ComaRenderer::releaseRenderTarget()
{
	if (renderTarget)renderTarget->Release();
}

bool ComaRenderer::run()
{
	if (!initialized || running)
		return false;
	timer->start();
	running = true;
	return true;
}
bool ComaRenderer::pause()
{
	if (!initialized || !running)
		return false;
	timer->stop();
	running = false;
	return true;
}
bool ComaRenderer::update()
{
	if (!initialized || !running)
		return false;

	//Time Process
	timer->tick();
	deltaTime = timer->getDeltaTime();
	if (deltaTime > maxFrameTime)
		deltaTime = maxFrameTime;
	else if (deltaTime < minFrameTime)
	{
		Sleep((DWORD)((minFrameTime-deltaTime)*1000));
		deltaTime = minFrameTime;
	}
	frameCount++;
	fps = (double)frameCount / timer->getRunningTime();
	//RenderProcess
	renderTarget->BeginDraw();
	renderTarget->Clear(backgroundColor);

	if (renderTarget->EndDraw() != S_OK)
	{
		restoreDevice();
		if (renderTarget->EndDraw() != S_OK)
		{
			return false;
		}
	}
	
	return true;
}
bool ComaRenderer::resetSize()
{
	if (!initialized)
		return false;
	RECT rect;
	GetClientRect(targetWindow, &rect);

	if(renderTarget->Resize(D2D1::SizeU(rect.right, rect.bottom)) == S_OK)
		return true;
	return false;
}
void ComaRenderer::restoreDevice()
{
	pause();
	releaseRenderTarget();
	Sleep(100);
	createRenderTarget(targetWindow);
	run();
}

//TODO: 이벤트 제작+디바이스 로스트 구현, 