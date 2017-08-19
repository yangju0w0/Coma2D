/*
* filename	GameObject.cpp
* fileinfo	게임 표시 객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "GameObject.h"

COMA_USING_NS

b2World* GameObject::physicsWorld_ = nullptr;
float GameObject::pixelPerMeter_ = 20;
float GameObject::meterPerPixel_ = 1 / 20.f;

GameObject::GameObject()
	:DisplayObjectContainer(), usePhysics_(false), collider_(nullptr), body_(nullptr)
{
	collider_ = new b2PolygonShape();

	//Point pos = localToWorld(Point{ getLocalPosition().x+ getLocalSize().width/2, getLocalPosition().y + getLocalSize().height/2 });
	//Point pos2 = localToWorld(getLocalPosition());

	bodyDef_.type = b2_dynamicBody;
	//bodyDef->position.Set(pos.x * meterPerPixel, pos.y * meterPerPixel);

	//((b2PolygonShape*)collider)->SetAsBox((pos.x - pos2.x)*meterPerPixel, (pos.y - pos2.y)*meterPerPixel);

	fixtureDef_.friction = 0.3f;
	fixtureDef_.density = 1.0f;
}

GameObject::~GameObject()
{
	if (collider_) delete collider_;
}

void GameObject::SetPhysicsWorld(b2World* world)
{
	physicsWorld_ = world;
}

void GameObject::SetPixelPerMeter(float pixel)
{
	pixelPerMeter_ = pixel;
	meterPerPixel_ = 1.f / pixel;
}

void GameObject::CreatePhysics(bool setDefault)
{
	if (!physicsWorld_)
	{
		return;
	}

	if (setDefault)
	{
		Point pos = LocalToWorld(Point{ GetLocalPosition().x + GetLocalSize().width / 2, GetLocalPosition().y + GetLocalSize().height / 2 });
		Point pos2 = LocalToWorld(GetLocalPosition());
		bodyDef_.position.Set(pos.x * meterPerPixel_, pos.y * meterPerPixel_);
		((b2PolygonShape*)collider_)->SetAsBox((pos.x - pos2.x) * meterPerPixel_, (pos.y - pos2.y) * meterPerPixel_);
	}
	body_ = physicsWorld_->CreateBody(&bodyDef_);
	fixtureDef_.shape = collider_;
	body_->CreateFixture(&fixtureDef_);
	usePhysics_ = true;
}

void GameObject::DestroyPhysics()
{
	usePhysics_ = false;
}

void GameObject::SetBodyType(b2BodyType type)
{
	bodyDef_.type = type;
}

void GameObject::SetColliderPosition(Point point)
{
	Point pos = LocalToWorld(point);
	bodyDef_.position.Set(pos.x * meterPerPixel_, pos.y * meterPerPixel_);
}

void GameObject::SetColliderPosition(float x, float y)
{
	SetColliderPosition(Point{ x, y });
}

void GameObject::SetBoxCollider(float width, float height)
{
	if (collider_)
	{
		delete collider_;
	}
	collider_ = new b2PolygonShape();
	Point pos = LocalToWorld(Point{ 0, 0 });
	Point pos2 = LocalToWorld(Point{ width, height });
	((b2PolygonShape*)collider_)->SetAsBox((pos2.x - pos.x) * meterPerPixel_, (pos2.y - pos.y) * meterPerPixel_);
}

void GameObject::SetCircleCollider(float radius)
{
	if (collider_)
	{
		delete collider_;
	}
	collider_ = new b2CircleShape();
	Point pos = LocalToWorld(Point{ 0, 0 });
	Point pos2 = LocalToWorld(Point{ radius, radius });
	((b2CircleShape*)collider_)->m_radius = ((pos2.x - pos.x) + (pos2.y - pos.y)) / 2 * meterPerPixel_;
}

void GameObject::SetCollider(b2Shape* shape)
{
	if (collider_)
	{
		delete collider_;
	}
	collider_ = shape;
}

void GameObject::SetDensity(float value)
{
	fixtureDef_.density = value;
}

void GameObject::SetFriction(float value)
{
	fixtureDef_.friction = value;
}

void GameObject::SetRestitution(float value)
{
	fixtureDef_.restitution = value;
}

void GameObject::Update(double deltaTime)
{
	if (body_ && IsBadReadPtr(body_, MAX_PATH))
	{
		return;
	}

	DisplayObjectContainer::Update(deltaTime);
	if (!usePhysics_ || IsBadReadPtr(body_, MAX_PATH))
	{
		return;
	}
	b2Vec2 position = body_->GetPosition();
	float32 angle = body_->GetAngle();
	this->SetPosition(position.x * pixelPerMeter_, position.y * pixelPerMeter_);
	this->SetRotation(angle*(180.0f / PI));

}