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
	ResourceManagerEvent(const std::string& type, ResourceManager* manager, int resourceNumbers = 0, int processedResourceNumbers = 0);
	virtual ~ResourceManagerEvent();

public:
	static const std::string LOAD_BEGIN;
	static const std::string LOADING;
	static const std::string LOAD_COMPLETE;

	static const std::string UNLOAD_BEGIN;
	static const std::string UNLOADING;
	static const std::string UNLOAD_COMPLETE;

public:
	//처리될 리소스 개수
	int GetResourceNumbers() const { return resourceNumbers_; }

	//처리되지 않은 리소스 개수
	int GetLeftResourceNumbers() const { return leftResourceNumbers_; }

	//처리된 리소스 개수
	int GetProcessedResourceNumbers() const { return processedResourceNumbers_; }

	//진행도 (0.0f ~ 1.0f)
	float GetProgress() const { return (float)processedResourceNumbers_ / (float)resourceNumbers_; }

	ResourceManager* GetTarget() const { return target_; }

private:
	int resourceNumbers_;
	int leftResourceNumbers_;
	int processedResourceNumbers_;
	ResourceManager* target_;
};

COMA_END