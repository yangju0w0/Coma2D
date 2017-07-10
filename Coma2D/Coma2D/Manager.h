/*
* filename	Manager.h
* fileinfo	매니저 추상클래스의 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "comaUtil.h"
#include "EventDispatcher.h"

COMA_NS_BEGIN

class Manager
	:public EventDispatcher
{
public:
	Manager();
	virtual ~Manager();
};

COMA_END