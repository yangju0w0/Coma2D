#pragma once
namespace Coma2D
{
	class Event
	{
	public:
		Event(const char* type);
		virtual ~Event();
		const char* getType()
		{
			return type;
		}
	private:
		const char* type;
	};
}