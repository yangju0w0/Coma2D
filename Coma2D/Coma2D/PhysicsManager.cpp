#include "PhysicsManager.h"

COMA_USING_NS

PhysicsManager::PhysicsManager()
	:world_(nullptr), pixelPerMeter_(0)
{
}


PhysicsManager::~PhysicsManager()
{
	delete world_;
}

void PhysicsManager::InitBox2D(Vector gravity)
{
	if (pixelPerMeter_ == 0)
	{
		SetPixelPerMeter(20);
	}
	if (world_)
	{
		return;
	}
	world_ = new b2World(gravity);
	GameObject::setPhysicsWorld(world_);
}

b2World* PhysicsManager::GetWorld() const
{
	return world_;
}

bool PhysicsManager::SetGravity(Vector gravity) const
{
	if (world_)
	{
		world_->SetGravity(gravity);
		return true;
	}
	return false;
}

void PhysicsManager::SetPixelPerMeter(float pixels)
{
	pixelPerMeter_ = pixels;
	GameObject::setPixelPerMeter(pixels);
}

void PhysicsManager::NextStep(double deltaTime) const
{
	world_->Step(deltaTime, 3, 1);
}