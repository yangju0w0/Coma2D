/*
* filename	ComaRenderer.cpp
* fileinfo	그래픽 렌더링 기능 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "ComaRenderer.h"
#include "RendererEvent.h"
COMA_USING_NS

ComaRenderer::ComaRenderer()
	:initialized_(false), running_(false), deltaTime_(0.0), fps_(0.0f), frameCount_(0), factory_(nullptr), renderTarget_(nullptr), timer_(nullptr), maxFrameTime_(0.05f), minFrameTime_(0.01f)
{
	timer_ = new RenderTimer();
	backgroundColor_ = D2D1::ColorF(0.0f, 0.0f, 0.0f);
}

ComaRenderer::~ComaRenderer()
{
	DispatchEvent(new RendererEvent(RendererEvent::UNLOAD_RESOURCES_REQ, this));
	if (factory_)factory_->Release();
	if (renderTarget_)renderTarget_->Release();
	delete timer_;
}

bool ComaRenderer::InitRenderer(HWND hWnd)
{
	if (initialized_)
	{
		return false;
	}
	
	if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_) != S_OK)
	{
		return false;
	}
	if (!CreateRenderTarget(hWnd))
	{
		return false;
	}

	timer_->reset();

	targetWindow_ = hWnd;
	initialized_ = true;
	return true;
}

bool ComaRenderer::CreateRenderTarget(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	if (factory_->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&renderTarget_) != S_OK)
	{
		return false;
	}
	DispatchEvent(new RendererEvent(RendererEvent::RENDER_TARGET_CREATED, this));
	return true;
}
void ComaRenderer::ReleaseRenderTarget()
{
	if (renderTarget_)
	{
		DispatchEvent(new RendererEvent(RendererEvent::UNLOAD_RESOURCES_REQ, this));
		renderTarget_->Release();
		DispatchEvent(new RendererEvent(RendererEvent::RENDER_TARGET_RELEASED, this));
	}
}

bool ComaRenderer::Run()
{
	if (!initialized_ || running_)
	{
		return false;
	}

	timer_->start();
	running_ = true;
	DispatchEvent(new RendererEvent(RendererEvent::RUN, this));
	return true;
}
bool ComaRenderer::Pause()
{
	if (!initialized_ || !running_)
	{
		return false;
	}

	timer_->stop();
	running_ = false;
	DispatchEvent(new RendererEvent(RendererEvent::PAUSED, this));
	return true;
}
bool ComaRenderer::Update()
{
	if (!initialized_ || !running_)
	{
		return false;
	}

	//Time Process
	timer_->tick();
	deltaTime_ = timer_->getDeltaTime();
	
	if (deltaTime_ > maxFrameTime_)
	{
		deltaTime_ = maxFrameTime_;
	}
	else if (deltaTime_ < minFrameTime_)
	{
		Sleep((DWORD)((minFrameTime_ - deltaTime_) * 1000));
		deltaTime_ = minFrameTime_;
	}

	fps_ = 1 / (float)deltaTime_;
	DispatchEvent(new RendererEvent(RendererEvent::UPDATE, this));

	//RenderProcess
	renderTarget_->BeginDraw();
	renderTarget_->Clear(backgroundColor_);

	DispatchEvent(new RendererEvent(RendererEvent::RENDER, this));

	// 실패할 경우 디바이스 복구 후 1회 재시도
	if (renderTarget_->EndDraw() != S_OK)
	{
		RestoreDevice();
		if (renderTarget_->EndDraw() != S_OK)
		{
			return false;
		}
	}

	frameCount_++;
	return true;
}
bool ComaRenderer::ResetSize()
{
	if (!initialized_)
	{
		return false;
	}
	RECT rect;
	GetClientRect(targetWindow_, &rect);

	return renderTarget_->Resize(D2D1::SizeU(rect.right, rect.bottom)) == S_OK;
}
void ComaRenderer::RestoreDevice()
{
	Pause();
	ReleaseRenderTarget();
	Sleep(100);
	DispatchEvent(new RendererEvent(RendererEvent::LOAD_RESOURCE_REQ, this));
	CreateRenderTarget(targetWindow_);
	Run();
}