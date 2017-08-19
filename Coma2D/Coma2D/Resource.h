/*
* filename	Resource.h
* fileinfo	리소스 인터페이스
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "comaUtil.h"

COMA_NS_BEGIN

class Resource
{
public:
	virtual bool LoadResource() = 0;
	virtual bool UnloadResource() = 0;
	virtual bool IsLoaded() = 0;

	//모든 리소스는 로드되지 않은 상태로 존재할 수 있다.
};

COMA_END