#include "stdafx.h"
#include "ComaRenderer.h"
using namespace Coma2D;


ComaRenderer::ComaRenderer()
	:factory(NULL), renderTarget(NULL), initialized(false)
{
}

ComaRenderer::~ComaRenderer()
{
}

bool ComaRenderer::initRenderer(HWND hWnd)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK)
		return false;
	RECT rect;
	GetClientRect(hWnd, &rect);
	resizeRenderTarget();
	initialized = true;
	this->hWnd = hWnd;
	return true;
}
void ComaRenderer::resizeRenderTarget()
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