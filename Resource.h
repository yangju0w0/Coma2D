#pragma once
#include "comaUtil.h"

COMA_NS_BEGIN

class Resource
{
public:
	virtual bool loadResource() = 0;
	virtual bool unloadResource() = 0;
	virtual bool isLoaded() = 0;

	//모든 리소스는 로드되지 않은 상태로 존재할 수 있다.
};

COMA_END