/*
* filename	SceneManager.cpp
* fileinfo	장면 관리 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "SceneManager.h"
#include "SceneEvent.h"
COMA_USING_NS

SceneManager::SceneManager()
	:scene_(nullptr)
{
}


SceneManager::~SceneManager()
{
	if(scene_)
	{
		delete scene_;
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	DisplayObject::_setWorld(scene);

	if(this->scene_)
	{
		delete this->scene_;
	}
	this->scene_ = scene;

	scene->DispatchEvent(new SceneEvent(SceneEvent::CHANGED, scene));
}