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