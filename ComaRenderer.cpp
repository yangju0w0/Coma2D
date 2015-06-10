#include "stdafx.h"
#include "ComaRenderer.h"
using namespace coma2d;


ComaRenderer::ComaRenderer()
	:initialized(false), running(false)
{
	backgroundColor = D2D1::ColorF(0.0f, 0.0f, 0.0f);
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
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
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

	running = true;
	return true;
}
bool ComaRenderer::pause()
{
	if (!initialized || !running)
		return false;

	running = false;
	return true;
}
bool ComaRenderer::update()
{
	if (!initialized || !running)
		return false;

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

	renderTarget->Resize(D2D1::SizeU(rect.right, rect.bottom));
}
void ComaRenderer::restoreDevice()
{
	releaseRenderTarget();
	Sleep(100);
	createRenderTarget(targetWindow);
}