#pragma once
#include "Event.h"
#include "ComaRenderer.h"
namespace Coma2D
{
	class RendererEvent :
		public Event
	{
	public:
		RendererEvent(std::string type, ComaRenderer* target);
		~RendererEvent();

		static const std::string UPDATE;
		static const std::string DRAW;

		float deltaTime(){ return renderer->getDeltaTime(); }
		ID2D1HwndRenderTarget* getRenderTarget(){ return renderer->getRenderTarget(); }
	private:
		ComaRenderer* renderer;
	};
}