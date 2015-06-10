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
}

bool ComaRenderer::initRenderer(HWND hWnd)
{
	if (initialized)
		return false;

	if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory) != S_OK)
		return false;
	RECT rect;
	GetClientRect(hWnd, &rect);

	if (factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
		) != S_OK)
		return false;

	targetWindow = hWnd;
	initialized = true;
	return true;
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
	renderTarget->EndDraw();

	return true;
}
bool ComaRenderer::resetSize()
{
	renderTarget->Release();

	RECT rect;
	GetClientRect(targetWindow, &rect);

	if (factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(targetWindow, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
		) != S_OK)
		return false;

	return true;
}

