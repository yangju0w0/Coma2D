#pragma once
#include <string>
namespace Coma2D
{
	class Event
	{
	public:
		Event(std::string type);
		virtual ~Event();
		std::string getType()
		{
			return type;
		}
	private:
		std::string type;
	};
}