/*
* filename	RendererEvent.h
* fileinfo	렌더링 기능 클래스의 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/


#pragma once
#include "Event.h"
#include "ComaRenderer.h"

COMA_NS_BEGIN

class RendererEvent :
	public Event
{
public:
	RendererEvent(const std::string& type, ComaRenderer* target);
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
	ID2D1HwndRenderTarget* getRenderTarget(){ return targetRenderer->getRenderTarget(); }
	ComaRenderer* getTarget(){ return targetRenderer; }
private:
	ComaRenderer* targetRenderer;
};

COMA_END