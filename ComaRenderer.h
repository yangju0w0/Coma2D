#pragma once
#include "EventDispatcher.h"
#include <d2d1.h>
namespace coma2d
{
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
		//Renderer Status Getter
		bool isInitialized(){ return initialized; }
		bool isRunning(){ return running; }

		//Renderer Attributes Setter
		void setBackgroundColor(D2D1_COLOR_F color) { backgroundColor = color; }
		void setBackgroundColor(float r, float g, float b) { backgroundColor = D2D1::ColorF(r, g, b); }

	private:
		//Renderer Status
		bool initialized;
		bool running;

	private:
		//Renderer Essencials
		ID2D1Factory* factory;
		ID2D1HwndRenderTarget* renderTarget;
		HWND targetWindow;

		//Renderer Attributes
		D2D1_COLOR_F backgroundColor;
	};
}