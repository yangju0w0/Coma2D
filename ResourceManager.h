#pragma once
#include "Manager.h"
#include "ComaRenderer.h"
#include "Resource.h"
#include <vector>
#include <thread>
namespace coma2d
{
	class ResourceManager :
		public Manager
	{
	public:
		ResourceManager(ComaRenderer* renderer);
		~ResourceManager();

		void registerResource(Resource* resource);

		void loadAllResources();
		void loadAllResources(std::string type);
		void unloadAllResources();
		void unloadAllResources(std::string type);

	private:
		std::vector<Resource*> resourceList;
	};
}