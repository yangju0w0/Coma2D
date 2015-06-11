#include "stdafx.h"
#include "RendererEvent.h"
using namespace coma2d;

const std::string RendererEvent::UPDATE					= "renderer_update";
const std::string RendererEvent::RENDER					= "renderer_render";
const std::string RendererEvent::PAUSED					= "renderer_renderPaused";
const std::string RendererEvent::RUN					= "renderer_renderRun";
const std::string RendererEvent::UNLOAD_RESOURCES_REQ	= "renderer_unloadResourceRequest";
const std::string RendererEvent::LOAD_RESOURCE_REQ		= "renderer_loadResourceRequest";
const std::string RendererEvent::RENDER_TARGET_CREATED	= "renderer_renderTargetCreated";
const std::string RendererEvent::RENDER_TARGET_RELEASED = "renderer_renderTargetReleased";

RendererEvent::RendererEvent(std::string type, ComaRenderer* target)
	:Event(type)
{
	targetRenderer = target;
}


RendererEvent::~RendererEvent()
{
}
