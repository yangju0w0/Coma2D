/*
* filename	SceneManager.h
* fileinfo	장면 관리 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Manager.h"
#include "Scene.h"

COMA_NS_BEGIN

class SceneManager :
	public Manager
{
public:
	SceneManager();
	virtual ~SceneManager();
public:
	void ChangeScene(Scene* scene);
	Scene* GetScene() const { return scene_; }

private:
	Scene* scene_;
};

COMA_END