#pragma once
#include "Event.h"
#include "ComaRenderer.h"
namespace coma2d
{
	class RendererEvent :
		public Event
	{
	public:
		RendererEvent(std::string type, ComaRenderer* target);
		virtual ~RendererEvent();

	public:
		static const std::string UPDATE;
		static const std::string RENDER;
		static const std::string PAUSED;
		static const std::string RUN;
		static const std::string UNLOAD_RESOURCES_REQ;
		static const std::string LOAD_RESOURCE_REQ;
		static const std::string RENDER_TARGET_RELEASED;
		static const std::string RENDER_TARGET_CREATED;
		
	public:
		double deltaTime(){ return targetRenderer->getDeltaTime(); }
		ID2D1HwndRenderTarget* getRenderTarget(){ targetRenderer->getRenderTarget(); }
		ComaRenderer* getTarget(){ return targetRenderer; }
	private:
		ComaRenderer* targetRenderer;
	};
}