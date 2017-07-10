/*
* filename	SceneManager.cpp
* fileinfo	장면 관리 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "SceneManager.h"
#include "SceneEvent.h"
COMA_USING_NS

SceneManager::SceneManager()
	:scene(nullptr)
{
}


SceneManager::~SceneManager()
{
	if(scene) delete scene;
}

void SceneManager::changeScene(Scene* scene)
{
	DisplayObject::_setWorld(scene);
	if(this->scene) delete this->scene;
	this->scene = scene;
	scene->dispatchEvent(new SceneEvent(SceneEvent::CHANGED, scene));
}