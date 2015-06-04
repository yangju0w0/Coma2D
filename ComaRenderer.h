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
		void render();

		//Status Getter
		bool isInitialized(){ return initialized; }
		float getTotalTime(){ return timer->getTotalTime(); }
		float getDeltaTime(){ return timer->getDeltaTime(); }
		ID2D1HwndRenderTarget* getRenderTarget(){ return renderTarget; }

		//Status Setter
		void setBackgroundColor(float r, float g, float b, float a = 1.0f){ setBackgroundColor(D2D1::ColorF(r, g, b, a)); };
		void setBackgroundColor(D2D1::ColorF color) { backgroundColor = color; }

		//Status Changer
		void resizeWindow();

	protected:
		//Renderer Essencials
		ID2D1Factory* factory;
		ID2D1HwndRenderTarget* renderTarget;
		GameTimer* timer;
	private:
		//Renderer Status
		HWND hWnd;
		bool initialized;
		D2D1_COLOR_F backgroundColor;

	private:
		void createRenderTarget(HWND hWnd);
	};
}