#pragma once
#include "Event.h"
#include "ResourceManager.h"
namespace coma2d
{
	class ResourceManagerEvent :
		public Event
	{
	public:
		ResourceManagerEvent(std::string type, ResourceManager* target, int maxResourceNum=0, int loadedResourceNum=0);
		virtual ~ResourceManagerEvent();
	public:
		static const std::string BEGIN_LOAD;
		static const std::string END_LOAD;
		static const std::string LOADING;

	public:
		ResourceManager* getTarget(){ return target; }
		int maxResourceNum(){ return maxResourceNumber; }
		int loadedResourceNum(){ return loadedResourceNumber; }
	private:
		ResourceManager* target;
		int maxResourceNumber;
		int loadedResourceNumber;
	};

}