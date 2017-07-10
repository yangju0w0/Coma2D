/*
* filename	InteractiveObject.h
* fileinfo	상호작용 표시 객체 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "comaUtil.h"
#include "DisplayObject.h"
COMA_NS_BEGIN

class InteractiveObject :
	public DisplayObject
{
public:
	InteractiveObject();
	virtual ~InteractiveObject();
};

COMA_END