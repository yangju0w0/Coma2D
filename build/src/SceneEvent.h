#pragma once
#include "Event.h"
#include "Scene.h"

COMA_NS_BEGIN

class SceneEvent :
	public Event
{
public:
	SceneEvent(std::string type, Scene* target);
	virtual ~SceneEvent();

public:
	static const std::string CHANGED;
	Scene* getScene(){ return scene; }
private:
	Scene* scene;
};

COMA_END