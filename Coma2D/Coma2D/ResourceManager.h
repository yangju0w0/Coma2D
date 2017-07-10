/*
* filename	ResourceManager.h
* fileinfo	府家胶 包府 努贰胶狼 备泅 颇老
* author	林清剧 (Heonyang Ju)
*/

#pragma once
#include "Manager.h"
#include "Resource.h"
#include <vector>
COMA_NS_BEGIN

class ResourceManager :
	public Manager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourceManager* push(Resource* resource);
	bool clear();

	bool loadResources();
	bool unloadResources();

	int getResourceNumbers(){ return resourceList.size(); }
	bool isLoading(){ return loading; }

private:
	std::vector<Resource*> resourceList;
	bool loading;

private:
	void completeListener(Event* event);
};

COMA_END