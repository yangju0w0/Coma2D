/*
* filename	RendererEvent.cpp
* fileinfo	렌더링 기능 클래스의 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "RendererEvent.h"
COMA_USING_NS

const std::string RendererEvent::UPDATE					= "renderer_update";
const std::string RendererEvent::RENDER					= "renderer_render";
const std::string RendererEvent::PAUSED					= "renderer_renderPaused";
const std::string RendererEvent::RUN					= "renderer_renderRun";
const std::string RendererEvent::UNLOAD_RESOURCES_REQ	= "renderer_unloadResourceRequest";
const std::string RendererEvent::LOAD_RESOURCE_REQ		= "renderer_loadResourceRequest";
const std::string RendererEvent::RENDER_TARGET_CREATED	= "renderer_renderTargetCreated";
const std::string RendererEvent::RENDER_TARGET_RELEASED = "renderer_renderTargetReleased";

RendererEvent::RendererEvent(const std::string& type, ComaRenderer* target)
	:Event(type), targetRenderer(target)
{}


RendererEvent::~RendererEvent()
{
}
