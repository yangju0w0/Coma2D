/*
* filename	ResourceManagerEvent.h
* fileinfo	리소스 매니저 클래스의 이벤트 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Event.h"
#include "ResourceManager.h"
COMA_NS_BEGIN

class ResourceManagerEvent :
	public Event
{
public:
	ResourceManagerEvent(std::string type, ResourceManager* manager, int resourceNumbers=0, int processedResourceNumbers = 0);
	virtual ~ResourceManagerEvent();

public:
	static const std::string LOAD_BEGIN;
	static const std::string LOADING;
	static const std::string LOAD_COMPLETE;

	static const std::string UNLOAD_BEGIN;
	static const std::string UNLOADING;
	static const std::string UNLOAD_COMPLETE;
public:
	int getResourceNumbers(){ return resourceNumbers; }	//처리될 리소스 개수
	int getLeftResourceNumbers(){ return leftResourceNumbers; }	//처리되지 않은 리소스 개수
	int getProcessedResourceNumbers(){ return processedResourceNumbers; } //처리된 리소스 개수
	float getProgress(){ return (float)processedResourceNumbers / (float)resourceNumbers; } //진행도 (0.0f ~ 1.0f)
	ResourceManager* getTarget(){ return target; }//타겟
private:
	int resourceNumbers;
	int leftResourceNumbers;
	int processedResourceNumbers;
	ResourceManager* target;
};

COMA_END