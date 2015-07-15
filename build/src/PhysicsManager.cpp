#include "PhysicsManager.h"

COMA_USING_NS

PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
}


void PhysicsManager::initBox2D(Vector gravity)
{
	if (world)
		return;
	world = new b2World(gravity);
}

void PhysicsManager::nextStep(double deltaTime)
{
	world->Step(deltaTime, 8, 3);
}