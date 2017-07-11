/*
* filename	SpriteEvent.h
* fileinfo	애니메이션 스프라이트 클래스 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Event.h"
#include "Sprite.h"

COMA_NS_BEGIN

class SpriteEvent :
	public Event
{
public:
	SpriteEvent(const std::string& type, Sprite* target);
	virtual ~SpriteEvent();

	static const std::string ENTER_FRAME;
	static const std::string STOPPED;
	static const std::string PLAY;

	Sprite* GetTarget() const { return target_; }
	unsigned int GetCurrentFrame() const { return target_->getCurrentFrame(); }
	unsigned int GetTotalFrame() const { return target_->getTotalFrame(); }

private:
	Sprite* target_;
};

COMA_END