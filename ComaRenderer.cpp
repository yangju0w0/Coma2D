#include "stdafx.h"
#include "ComaRenderer.h"
using namespace Coma2D;


ComaRenderer::ComaRenderer()
	:factory(NULL), renderTarget(NULL), initialized(false)
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
void ComaRenderer::render()
{
	renderTarget->BeginDraw();
	renderTarget->Clear(backgroundColor);
	renderTarget->EndDraw();
}