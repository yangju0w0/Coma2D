#pragma once
#include "Manager.h"

COMA_NS_BEGIN

class PhysicsManager :
	public Manager
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();

	void initBox2D(Vector gravity);
	b2World* getWorld(){ return world; }
	void nextStep(double deltaTime);
	void setGravity(Vector gravity){ if (world) world->SetGravity(gravity); }
private:
	b2World* world;
};

COMA_END