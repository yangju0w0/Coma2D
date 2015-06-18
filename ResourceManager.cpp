#include "stdafx.h"
#include "ResourceManager.h"
#include "ResourceManagerEvent.h"
using namespace coma2d;

ResourceManager::ResourceManager(ComaRenderer* renderer)
{
	
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::registerResource(Resource* resource)
{
	if (resource)
		resourceList.push_back(resource);
}

void loadAllResourcesTrd(ResourceManager* manager, std::vector<Resource*> resourceList)
{
	int maxCount = 0;
	int currCount = 0;
	for (unsigned int i = 0; i < resourceList.size(); i++)
	{
		if (!resourceList[i])
		{
			resourceList.erase(resourceList.begin() + i);
			i--;
			continue;
		}
		maxCount++;
	}
	manager->dispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::BEGIN_LOAD, manager, maxCount, 0));
	for (unsigned int i = 0; i < resourceList.size(); i++)
	{
		if (!resourceList[i])
		{
			resourceList.erase(resourceList.begin() + i);
			i--;
			continue;
		}
		resourceList[i]->loadResource();
		currCount++;
		manager->dispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::LOADING, manager, maxCount, currCount));
	}
	manager->dispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::END_LOAD, manager, maxCount, currCount));
}
void loadAllResourcesTypeTrd(ResourceManager* manager, std::vector<Resource*> resourceList, std::string type)
{
	int maxCount = 0;
	int currCount = 0;
	for (unsigned int i = 0; i < resourceList.size(); i++)
	{
		if (!resourceList[i])
		{
			resourceList.erase(resourceList.begin() + i);
			i--;
			continue;
		}
		if (resourceList[i]->getResourceType() == type)
			maxCount++;
	}
	manager->dispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::BEGIN_LOAD, manager, maxCount, 0));
	for (unsigned int i = 0; i < resourceList.size(); i++)
	{
		if (!resourceList[i])
		{
			resourceList.erase(resourceList.begin() + i);
			i--;
			continue;
		}
		if (resourceList[i]->getResourceType() == type)
			resourceList[i]->loadResource();
		currCount++;
		manager->dispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::LOADING, manager, maxCount, currCount));
	}
	manager->dispatchEvent(new ResourceManagerEvent(ResourceManagerEvent::END_LOAD, manager, maxCount, currCount));
}
void unloadAllResourcesTrd(std::vector<Resource*> resourceList)
{
	for (unsigned int i = 0; i < resourceList.size(); i++)
	{
		if (!resourceList[i])
		{
			resourceList.erase(resourceList.begin() + i);
			i--;
			continue;
		}
		resourceList[i]->unloadResource();
	}
}
void unloadAllResourcesTypeTrd(std::vector<Resource*> resourceList, std::string type)
{
	for (unsigned int i = 0; i < resourceList.size(); i++)
	{
		if (!resourceList[i])
		{
			resourceList.erase(resourceList.begin() + i);
			i--;
			continue;
		}
		if (resourceList[i]->getResourceType() == type)
			resourceList[i]->unloadResource();
	}
}

void ResourceManager::loadAllResources()
{
	std::thread trd(loadAllResourcesTrd, this, std::ref(resourceList));
	trd.detach();
}
void ResourceManager::loadAllResources(std::string type)
{
	std::thread trd(loadAllResourcesTypeTrd,this, std::ref(resourceList), type);
	trd.detach();
}

void ResourceManager::unloadAllResources()
{
	std::thread trd(unloadAllResourcesTrd, std::ref(resourceList));
	trd.detach();
}
void ResourceManager::unloadAllResources(std::string type)
{
	std::thread trd(unloadAllResourcesTypeTrd, std::ref(resourceList), type);
	trd.detach();
}