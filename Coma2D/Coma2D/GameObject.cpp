/*
* filename	GameObject.cpp
* fileinfo	게임 표시 객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "GameObject.h"

COMA_USING_NS

b2World* GameObject::physicsWorld = nullptr;
float GameObject::pixelPerMeter = 20;
float GameObject::meterPerPixel = 1 / 20.f;

GameObject::GameObject()
	:DisplayObjectContainer(), usePhysics(false), collider(nullptr), body(nullptr)
{
	collider = new b2PolygonShape();

	//Point pos = localToWorld(Point{ getLocalPosition().x+ getLocalSize().width/2, getLocalPosition().y + getLocalSize().height/2 });
	//Point pos2 = localToWorld(getLocalPosition());

	bodyDef.type = b2_dynamicBody;
	//bodyDef->position.Set(pos.x * meterPerPixel, pos.y * meterPerPixel);

	//((b2PolygonShape*)collider)->SetAsBox((pos.x - pos2.x)*meterPerPixel, (pos.y - pos2.y)*meterPerPixel);

	fixtureDef.friction = 0.3f;
	fixtureDef.density = 1.0f;
}


GameObject::~GameObject()
{
	if(collider) delete collider;
}

void GameObject::setPhysicsWorld(b2World* world)
{
	physicsWorld = world;
}
void GameObject::setPixelPerMeter(float pixel)
{
	pixelPerMeter = pixel;
	meterPerPixel = 1.f / pixel;
}

void GameObject::createPhysics(bool setDefault)
{
	if (!physicsWorld)
		return;
	if (setDefault)
	{
		Point pos = localToWorld(Point{ getLocalPosition().x + getLocalSize().width / 2, getLocalPosition().y + getLocalSize().height / 2 });
		Point pos2 = localToWorld(getLocalPosition());
		bodyDef.position.Set(pos.x * meterPerPixel, pos.y * meterPerPixel);
		((b2PolygonShape*)collider)->SetAsBox((pos.x - pos2.x)*meterPerPixel, (pos.y - pos2.y)*meterPerPixel);
	}
	body = physicsWorld->CreateBody(&bodyDef);
	fixtureDef.shape = collider;
	body->CreateFixture(&fixtureDef);
	usePhysics = true;
}
void GameObject::destroyPhysics()
{
	usePhysics = false;
}

void GameObject::setBodyType(b2BodyType type)
{
	bodyDef.type = type;
}
void GameObject::setColliderPosition(Point point)
{
	Point pos = localToWorld(point);
	bodyDef.position.Set(pos.x*meterPerPixel, pos.y*meterPerPixel);
}
void GameObject::setColliderPosition(float x, float y)
{
	setColliderPosition(Point{ x, y });
}
void GameObject::setBoxCollider(float width, float height)
{
	if (collider) delete collider;
	collider = new b2PolygonShape();
	Point pos = localToWorld(Point{ 0, 0 });
	Point pos2 = localToWorld(Point{width, height});
	((b2PolygonShape*)collider)->SetAsBox((pos2.x - pos.x)*meterPerPixel, (pos2.y - pos.y)*meterPerPixel);
}
void GameObject::setCircleCollider(float radius)
{
	if (collider) delete collider;
	collider = new b2CircleShape();
	Point pos = localToWorld(Point{ 0, 0 });
	Point pos2 = localToWorld(Point{ radius, radius });
	((b2CircleShape*)collider)->m_radius = ((pos2.x - pos.x) + (pos2.y - pos.y))/2 * meterPerPixel;
}
void GameObject::setCollider(b2Shape* shape)
{
	if (collider) delete collider;
	collider = shape;
}

void GameObject::setDensity(float value)
{
	fixtureDef.density = value;
}
void GameObject::setFriction(float value)
{
	fixtureDef.friction = value;
}
void GameObject::setRestitution(float value)
{
	fixtureDef.restitution = value;
}


void GameObject::update(double deltaTime)
{
	if (body && IsBadReadPtr(body, MAX_PATH))
	{
		return;
	}
	DisplayObjectContainer::update(deltaTime);
	if (!usePhysics || IsBadReadPtr(body, MAX_PATH))
		return;
	b2Vec2 position = body->GetPosition();
	float32 angle = body->GetAngle();
	this->setPosition(position.x*pixelPerMeter, position.y*pixelPerMeter);
	this->setRotation(angle*(180.0f/PI));
	
}