#pragma once
#include "Manager.h"
#include "GameObject.h"

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
	void setPixelPerMeter(float pixels){ pixelPerMeter = pixels; GameObject::setPixelPerMeter(pixels); }
private:
	b2World* world;
	float pixelPerMeter;
};

COMA_END