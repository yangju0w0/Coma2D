/*
* filename	ComaRenderer.h
* fileinfo	그래픽 렌더링 기능 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "EventDispatcher.h"
#include "RenderTimer.h"
#include <d2d1.h>

COMA_NS_BEGIN

class ComaRenderer :
	public EventDispatcher
{
public:
	ComaRenderer();
	virtual ~ComaRenderer();

	//Renderer Controls
	bool InitRenderer(HWND hWnd);
	bool Run();
	bool Pause();

	bool Update();
	bool ResetSize(); //렌더 타겟의 사이즈를 윈도우에 맞게 변경한다.

public:
	//Renderer Essencials Getter
	ID2D1Factory* GetFactory() const { return factory_; }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return renderTarget_; }

	//Renderer Status Getter
	bool IsInitialized() const { return initialized_; }
	bool IsRunning() const { return running_; }

	double GetDeltaTime() const { return deltaTime_; }
	double GetTotalTime() const { return timer_->GetTotalTime(); }
	double GetRunningTime() const { return timer_->GetRunningTime(); }
	double GetPausedTime() const { return timer_->GetPausedTime(); }
	float GetFps() const { return fps_; }
	__int64 GetFrameCount() const { return frameCount_; }

	//Renderer Attributes Setter
	void SetBackgroundColor(D2D1_COLOR_F color) { backgroundColor_ = color; }
	void SetBackgroundColor(float r, float g, float b) { backgroundColor_ = D2D1::ColorF(r, g, b); }
	void SetMinFrameTime(float value) { minFrameTime_ = value; } //최소 렌더링 시간을 제한한다.
	void SetMaxFrameTime(float value) { maxFrameTime_ = value; } //최대 렌더링 시간을 제한한다.

	//Renderer Attributes Getter
	D2D1_COLOR_F GetBackgroundColor() const { return backgroundColor_; }
	float GetMinFrameTime() const { return minFrameTime_; }
	float GetMaxFrameTime() const { return maxFrameTime_; }

private:
	bool CreateRenderTarget(HWND hWnd);
	void ReleaseRenderTarget();
	void RestoreDevice();

private:
	//Renderer Essencials
	ID2D1Factory* factory_;
	ID2D1HwndRenderTarget* renderTarget_;
	HWND targetWindow_;
	RenderTimer* timer_;

	//Renderer Attributes
	D2D1_COLOR_F backgroundColor_;
	float maxFrameTime_;
	float minFrameTime_;

	//Renderer Status
	bool initialized_;
	bool running_;

	//Renderer Temp
	double deltaTime_;
	__int64 frameCount_;
	float fps_;
};

COMA_END