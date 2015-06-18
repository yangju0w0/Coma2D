#include "stdafx.h"
#include "ResourceManagerEvent.h"
using namespace coma2d;

const std::string ResourceManagerEvent::BEGIN_LOAD = "beginLoad";
const std::string ResourceManagerEvent::END_LOAD = "endLoad";
const std::string ResourceManagerEvent::LOADING = "loading";

ResourceManagerEvent::ResourceManagerEvent(std::string type, ResourceManager* target, int maxResourceNum, int loadedResourceNum)
	:Event(type),target(target), maxResourceNumber(maxResourceNum), loadedResourceNumber(loadedResourceNum)
{
	
}


ResourceManagerEvent::~ResourceManagerEvent()
{
}
