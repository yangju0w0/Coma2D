/*
* filename	ResourceManager.cpp
* fileinfo	府家胶 包府 努贰胶狼 备泅 颇老
* author	林清剧 (Heonyang Ju)
*/

#include "ResourceManager.h"
#include "ResourceManagerEvent.h"
#include <thread>

COMA_USING_NS

ResourceManager::ResourceManager() : loading_(false)
{
	SetEventListener(ResourceManagerEvent::LOAD_COMPLETE, BIND(ResourceManager::CompleteListener));
	SetEventListener(ResourceManagerEvent::UNLOAD_COMPLETE, BIND(ResourceManager::CompleteListener));
}

ResourceManager::~ResourceManager()
{
	Clear();
}

ResourceManager* ResourceManager::Push(Resource* resource)
{
	if (resource)
	{
		resourceList_.push_back(resource);
	}

	return this;
}

bool ResourceManager::Clear()
{
	resourceList_.clear();
	return true;
}

void thread_loadResources(ResourceManager* manager, std::vector<Resource*> resources)
{
	int resourceNumbers = resources.size();
	int loadedResourceNumbers = 0;
	manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::LOAD_BEGIN, manager, resourceNumbers, loadedResourceNumbers));

	for (auto& resource : resources)
	{
		if (resource)
		{
			if (!resource->IsLoaded())
			{
				resource->LoadResource();
			}
		}
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

	for (auto& resource : resources)
	{
		if (resource)
		{
			if (!resource->IsLoaded())
			{
				resource->UnloadResource();
			}
		}
		unloadedResourceNumbers++;
		manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::UNLOADING, manager, resourceNumbers, unloadedResourceNumbers));
	}

	manager->DispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::UNLOAD_COMPLETE, manager, resourceNumbers, unloadedResourceNumbers));
}

bool ResourceManager::LoadResources()
{
	if (loading_)
	{
		return false;
	}

	std::thread trd(thread_loadResources, this, resourceList_);
	trd.detach();
	loading_ = true;
	return true;
}

bool ResourceManager::UnloadResources()
{
	if (loading_)
	{
		return false;
	}

	std::thread trd(thread_unloadResources, this, resourceList_);
	trd.detach();
	loading_ = true;
	return true;
}

void ResourceManager::CompleteListener(const Event* event)
{
	loading_ = false;
}