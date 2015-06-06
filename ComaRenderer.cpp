#include "stdafx.h"
#include "ComaRenderer.h"
#include "RendererEvent.h"
using namespace Coma2D;


ComaRenderer::ComaRenderer()
	:factory(NULL), renderTarget(NULL), initialized(false), running(false)
{
	backgroundColor = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
}

ComaRenderer::~ComaRenderer()
{
}

bool ComaRenderer::initRenderer(HWND hWnd)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK)
		return false;
	createRenderTarget(hWnd);

	initialized = true;
	this->hWnd = hWnd;
	timer.reset();
	return true;
}
void ComaRenderer::createRenderTarget(HWND hWnd)
{
	if(renderTarget) renderTarget->Release();
	RECT rect;
	GetClientRect(hWnd, &rect);
	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
		);
}
void ComaRenderer::resizeWindow()
{
	createRenderTarget(hWnd);
}
bool ComaRenderer::run()
{
	if (isRunning())
		return false;
	timer.start();
	running = true;
	return true;
}
bool ComaRenderer::pause()
{
	if (!isRunning())
		return false;
	timer.stop();
	running = false;
	return true;
}
void ComaRenderer::render()
{
	if (!isRunning())
		return;
	timer.tick();
	dispatchEvent(new RendererEvent(RendererEvent::UPDATE, this));
	renderTarget->BeginDraw();
	renderTarget->Clear(backgroundColor);
	dispatchEvent(new RendererEvent(RendererEvent::DRAW, this));
	renderTarget->EndDraw();
	totalFrame++;
}

float ComaRenderer::getFPS()
{
	if (getTotalTime() == 0 || !isRunning())
		return 0;
	return totalFrame / getTotalTime();
}