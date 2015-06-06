#pragma once
#include "EventDispatcher.h"
#include "GameTimer.h"
#include <d2d1.h>
namespace Coma2D
{
	class ComaRenderer :
		public EventDispatcher
	{
	public:
		ComaRenderer();
		virtual ~ComaRenderer();

		bool initRenderer(HWND hWnd);
		bool run();
		bool pause();
		void render();

		//Status Getter
		bool isInitialized(){ return initialized; }
		bool isRunning(){ return running; }
		float getTotalTime(){ return timer.getTotalTime(); }
		float getDeltaTime(){ return timer.getDeltaTime(); }
		ID2D1HwndRenderTarget* getRenderTarget(){ return renderTarget; }

		//Status Setter
		void setBackgroundColor(float r, float g, float b, float a = 1.0f){ setBackgroundColor(D2D1::ColorF(r, g, b, a)); };
		void setBackgroundColor(D2D1::ColorF color) { backgroundColor = color; }

		//Status Changer
		void resizeWindow();
		float getFPS();
		

	protected:
		//Renderer Essencials
		ID2D1Factory* factory;
		ID2D1HwndRenderTarget* renderTarget;
		GameTimer timer;
	private:
		//Renderer Status
		HWND hWnd;
		bool initialized;
		bool running;
		D2D1_COLOR_F backgroundColor;
		__int64 totalFrame;
		
	private:
		void createRenderTarget(HWND hWnd);
	};
}


/*
TODO:	FPS 계산
		DisplayObject 연동 렌더링
		SceneManager
		최대 프레임 제한 설정
*/