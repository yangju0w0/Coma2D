#include "PhysicsManager.h"

COMA_USING_NS

PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
	delete world;
}


void PhysicsManager::initBox2D(Vector gravity)
{
	if (pixelPerMeter == 0)
		setPixelPerMeter(20);
	if (world)
		return;
	world = new b2World(gravity);
	GameObject::setPhysicsWorld(world);
}

void PhysicsManager::nextStep(double deltaTime)
{
	world->Step(deltaTime, 3, 1);
}