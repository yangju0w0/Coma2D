/*
* filename	SpriteEvent.cpp
* fileinfo	애니메이션 스프라이트 클래스 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "SpriteEvent.h"

COMA_USING_NS

const std::string SpriteEvent::ENTER_FRAME = "sprite_enterFrame";
const std::string SpriteEvent::STOPPED = "sprite_stopped";
const std::string SpriteEvent::PLAY = "sprite_play";

SpriteEvent::SpriteEvent(std::string type, Sprite* target)
:Event(type), target(target)
{
}


SpriteEvent::~SpriteEvent()
{
}
