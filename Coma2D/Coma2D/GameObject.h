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
	static void SetPhysicsWorld(b2World* world);
	static void SetPixelPerMeter(float pixel);

	b2Shape* GetCollider() const { return collider_; }
	b2Body* GetBody() const { return body_; }
	const b2BodyDef& GetBodyDef() const { return bodyDef_; }
	const b2FixtureDef& GetFixtureDef() const { return fixtureDef_; }

public:
	void CreatePhysics(bool setDefault = false);
	void DestroyPhysics();

	void SetBodyType(b2BodyType type);
	void SetColliderPosition(Point point);
	void SetColliderPosition(float x, float y);
	void SetBoxCollider(float width, float height);
	void SetCircleCollider(float radius);
	void SetCollider(b2Shape* shape);

	void SetBodyDef(b2BodyDef def) { this->bodyDef_ = def; }
	void SetFixtureDef(b2FixtureDef def) { this->fixtureDef_ = def; }

	void SetDensity(float value);
	void SetFriction(float value);
	void SetRestitution(float value);

	virtual void Update(double deltaTime);

	b2Shape* collider_;
	b2Body* body_;
	b2BodyDef bodyDef_;
	b2FixtureDef fixtureDef_;
private:
	static b2World*	physicsWorld_;
	static float pixelPerMeter_;
	static float meterPerPixel_;

	bool usePhysics_;
};

COMA_END