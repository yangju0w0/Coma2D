/*
* filename	ResourceManager.cpp
* fileinfo	府家胶 包府 努贰胶狼 备泅 颇老
* author	林清剧 (Heonyang Ju)
*/

#include "ResourceManager.h"
#include "ResourceManagerEvent.h"
#include <thread>

COMA_USING_NS

ResourceManager::ResourceManager() : loading(false)
{
	SetEventListener(ResourceManagerEvent::LOAD_COMPLETE, BIND(ResourceManager::completeListener));
	SetEventListener(ResourceManagerEvent::UNLOAD_COMPLETE, BIND(ResourceManager::completeListener));
}


ResourceManager::~ResourceManager()
{
	clear();
}

ResourceManager* ResourceManager::push(Resource* resource)
{
	if (resource)
		resourceList.push_back(resource);
	return this;
}

bool ResourceManager::clear()
{
	resourceList.clear();
	return true;
}

void thread_loadResources(ResourceManager* manager, std::vector<Resource*> resources)
{
	int resourceNumbers = resources.size();
	int loadedResourceNumbers = 0;
	manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::LOAD_BEGIN, manager, resourceNumbers, loadedResourceNumbers));
	for (unsigned int i = 0; i < resources.size(); i++)
	{
		if (resources[i])
			if (!resources[i]->isLoaded())
				resources[i]->loadResource();
		loadedResourceNumbers++;
		manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::LOADING, manager, resourceNumbers, loadedResourceNumbers));
	}
	manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::LOAD_COMPLETE, manager, resourceNumbers, loadedResourceNumbers));
}
void thread_unloadResources(ResourceManager* manager, std::vector<Resource*> resources)
{
	int resourceNumbers = resources.size();
	int unloadedResourceNumbers = 0;
	manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::UNLOAD_BEGIN, manager, resourceNumbers, unloadedResourceNumbers));
	for (unsigned int i = 0; i < resources.size(); i++)
	{
		if (resources[i])
			if (resources[i]->isLoaded())
				resources[i]->unloadResource();
		unloadedResourceNumbers++;
		manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::UNLOADING, manager, resourceNumbers, unloadedResourceNumbers));
	}
	manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::UNLOAD_COMPLETE, manager, resourceNumbers, unloadedResourceNumbers));
}

bool ResourceManager::loadResources()
{
	if (loading)
		return false;
	std::thread trd(thread_loadResources, this, resourceList);
	trd.detach();
	loading = true;
	return true;
}

bool ResourceManager::unloadResources()
{
	if (loading)
		return false;
	std::thread trd(thread_unloadResources, this, resourceList);
	trd.detach();
	loading = true;
	return true;
}

void ResourceManager::completeListener(Event* event)
{
	loading = false;
}