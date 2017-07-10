/*
* filename	SceneEvent.cpp
* fileinfo	장면(Scene) 클래스의 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "SceneEvent.h"

COMA_USING_NS

const std::string SceneEvent::CHANGED = "scene_sceneChanged";

SceneEvent::SceneEvent(const std::string& type, const Scene* scene)
:Event(type), scene(scene)
{

}


SceneEvent::~SceneEvent()
{
}
