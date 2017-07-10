/*
* filename	Scene.h
* fileinfo	장면 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "DisplayObjectContainer.h"

COMA_NS_BEGIN

class Scene :
	public DisplayObjectContainer
{
protected:
	Scene();
public:
	virtual ~Scene();
	bool _registerParent(DisplayObjectContainer* parent) { return false; }	//(오버라이드) Scene객체는 addChild로 추가할 수 없다.
	bool _unregisterParent(){ return false; }								//(오버라이드) Scene객체는 addChild로 추가할 수 없다.
};

COMA_END