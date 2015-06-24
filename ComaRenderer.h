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
	bool initRenderer(HWND hWnd);
	bool run();
	bool pause();

	bool update();
	bool resetSize();

public:
	//Renderer Essencials Getter
	ID2D1Factory* getFactory(){ return factory; }
	ID2D1HwndRenderTarget* getRenderTarget(){ return renderTarget; }
	RenderTimer* timer;

	//Renderer Status Getter
	bool isInitialized(){ return initialized; }
	bool isRunning(){ return running; }

	double getDeltaTime(){ return deltaTime; }
	double getTotalTime(){ return timer->getTotalTime(); }
	double getRunningTime(){ return timer->getRunningTime(); }
	double getPausedTime(){ return timer->getPausedTime(); }
	float getFps(){ return fps; }
	__int64 getFrameCount(){ return frameCount; }

	//Renderer Attributes Setter
	void setBackgroundColor(D2D1_COLOR_F color) { backgroundColor = color; }
	void setBackgroundColor(float r, float g, float b) { backgroundColor = D2D1::ColorF(r, g, b); }
	void setMinFrameTime(float value){ minFrameTime = value; }
	void setMaxFrameTime(float value){ maxFrameTime = value; }

	//Renderer Attributes Getter
	D2D1_COLOR_F getBackgroundColor(){ return backgroundColor; }
	float getMinFrameTime(){ return minFrameTime; }
	float getMaxFrameTime(){ return maxFrameTime; }

private:
	bool createRenderTarget(HWND hWnd);
	void releaseRenderTarget();
	void restoreDevice();
	
private:
	//Renderer Essencials
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	HWND targetWindow;

	//Renderer Attributes
	D2D1_COLOR_F backgroundColor;
	float maxFrameTime;
	float minFrameTime;

	//Renderer Status
	bool initialized;
	bool running;

	//Renderer Temp
	double deltaTime;
	__int64 frameCount;
	float fps;
};

COMA_END