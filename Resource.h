#pragma once
#include "EventDispatcher.h"
namespace coma2d
{
	class Resource :
		public EventDispatcher
	{
	protected:
		Resource(){};
		~Resource(){};
		
	public:
		virtual bool loadResource()=0;
		virtual bool unloadResource()=0;
		virtual bool isLoaded() = 0;
		virtual std::string getResourceType()=0;
	};
}