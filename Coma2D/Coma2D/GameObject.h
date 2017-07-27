/*
* filename	GameObject.h
* fileinfo	게임 표시 객체 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "DisplayObjectContainer.h"

COMA_NS_BEGIN

class GameObject :
	public DisplayObjectContainer
{
public:
	GameObject();
	virtual ~GameObject();

public:
	static void setPhysicsWorld(b2World* world);
	static void setPixelPerMeter(float pixel);

	b2Shape*		getcollider()	{ return collider; }
	b2Body*			getBody()		{ return body; }
	b2BodyDef		getBodyDef()	{ return bodyDef; }
	b2FixtureDef	getFixtureDef()	{ return fixtureDef; }

public:
	void createPhysics(bool setDefault=false);
	void destroyPhysics();

	void setBodyType(b2BodyType type);
	void setColliderPosition(Point point);
	void setColliderPosition(float x, float y);
	void setBoxCollider(float width, float height);
	void setCircleCollider(float radius);
	void setCollider(b2Shape* shape);


	void setBodyDef(b2BodyDef def)			{ this->bodyDef = def; }
	void setFixtureDef(b2FixtureDef def)	{ this->fixtureDef = def; }

	void setDensity(float value);
	void setFriction(float value);
	void setRestitution(float value);

	virtual void Update(double deltaTime);

	b2Shape*		collider;
	b2Body*			body;
	b2BodyDef		bodyDef;
	b2FixtureDef	fixtureDef;
private:
	static b2World*	physicsWorld;
	static float	pixelPerMeter;
	static float	meterPerPixel;
	
	bool usePhysics;
};

COMA_END