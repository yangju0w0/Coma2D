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
	double DeltaTime() const { return targetRenderer_->GetDeltaTime(); }
	ID2D1HwndRenderTarget* GetRenderTarget() const { return targetRenderer_->GetRenderTarget(); }
	ComaRenderer* GetTarget() const { return targetRenderer_; }
private:
	ComaRenderer* targetRenderer_;
};

COMA_END