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

	void InitBox2D(Vector gravity);
	b2World* GetWorld() const;
	void NextStep(double deltaTime) const;
	bool SetGravity(Vector gravity) const;
	void SetPixelPerMeter(float pixels);
private:
	b2World* world_;
	float pixelPerMeter_;
};

COMA_END