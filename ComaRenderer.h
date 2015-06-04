#pragma once
#include "EventDispatcher.h"
namespace Coma2D
{
	class ComaRenderer :
		public EventDispatcher
	{
	public:
		ComaRenderer();
		virtual ~ComaRenderer();
		bool initRenderer(HWND hWnd);
		bool isInitialized(){ return initialized; }

		void resizeRenderTarget();
	protected:
		ID2D1Factory* factory;
		ID2D1HwndRenderTarget* renderTarget;
	private:
		HWND hWnd;

		bool initialized;
	};
}