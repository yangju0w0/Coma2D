/*
* filename	ResourceManagerEvent.cpp
* fileinfo	리소스 매니저 클래스의 이벤트 구현 파일
* author	주헌양 (Heonyang Ju)
*/



#include "ResourceManagerEvent.h"

COMA_USING_NS

const std::string ResourceManagerEvent::LOAD_BEGIN		= "resourceManager_loadBegin";
const std::string ResourceManagerEvent::LOADING			= "resourceManager_loading";
const std::string ResourceManagerEvent::LOAD_COMPLETE	= "resourceManager_loadComplete";

const std::string ResourceManagerEvent::UNLOAD_BEGIN	= "resourceManager_unloadBegin";
const std::string ResourceManagerEvent::UNLOADING		= "resourceManager_unloading";
const std::string ResourceManagerEvent::UNLOAD_COMPLETE = "resourceManager_unloadComplete";

ResourceManagerEvent::ResourceManagerEvent(std::string type, ResourceManager* target, int resourceNumbers, int processedResourceNumbers)
	:Event(type), target(target), resourceNumbers(resourceNumbers), processedResourceNumbers(processedResourceNumbers), leftResourceNumbers(resourceNumbers - processedResourceNumbers)
{
}


ResourceManagerEvent::~ResourceManagerEvent()
{
}
