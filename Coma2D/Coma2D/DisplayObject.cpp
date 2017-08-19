/*
* filename	DisplayObject.cpp
* fileinfo	화면 표시 객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "DisplayObject.h"
#include "DisplayObjectContainer.h"
#include "Camera.h"
#include "ObjectEvent.h"
#include <algorithm>
COMA_USING_NS

DisplayObjectContainer* DisplayObject::world = nullptr;
ID2D1SolidColorBrush* DisplayObject::brush = nullptr;

DisplayObject::DisplayObject()
	:position_(Point{ 0, 0 }),
	scale_(Size{ 1, 1 }),
	rotation_(0),
	anchorPoint_(Point{ 0, 0 }),
	visible_(true),
	alpha_(1.0f),
	localSize_(Size{ 0, 0 }),
	localPoint_(Point{ 0, 0 }),
	drawOutline_(false),
	parentObject_(nullptr),
	camera_(nullptr)
{}

DisplayObject::~DisplayObject()
{
	if (GetParent())
	{
		GetParent()->RemoveChild(this);
	}
	if (drawOutline_)
	{
		if (brush)
		{
			brush->Release();
			brush = nullptr;
		}
	}
}

void DisplayObject::SetLocalSize(const Size& size)
{
	localSize_ = size;
	_TransformApply();
}

void DisplayObject::SetLocalSize(float width, float height)
{
	SetLocalSize(Size{ width, height });
}

void DisplayObject::SetLocalPosition(const Point& position)
{
	localPoint_ = position;
}

void DisplayObject::SetLocalPosition(float x, float y)
{
	SetLocalPosition(Point{ x, y });
}

void DisplayObject::_SetWorld(DisplayObjectContainer* world)
{
	DisplayObject::world = world;
}

void DisplayObject::SetCamera(Camera* camera)
{
	if (camera)
	{
		this->camera_ = camera;
		camera->_RegisterParent(parentObject_);
	}
}

void DisplayObject::UnsetCamera()
{
	if (this->camera_)
	{
		camera_->_UnregisterParent();
		camera_ = nullptr;
	}
}

void DisplayObject::SetPosition(float x, float y)
{
	SetPosition(Point{ x, y });
}

void DisplayObject::SetPosition(Point position)
{
	this->position_ = position;
	_TransformApply();
}

void DisplayObject::SetX(float value)
{
	this->position_.x = value;
	_TransformApply();
}

void DisplayObject::SetY(float value)
{
	this->position_.y = value;
	_TransformApply();
}

void DisplayObject::Translate(Point vector)
{
	this->position_.x += vector.x;
	this->position_.y += vector.y;
	_TransformApply();
}

void DisplayObject::Translate(float x, float y)
{
	Translate(D2D1::Point2F(x, y));
}

void DisplayObject::TranslateX(float value)
{
	this->position_.x += value;
	_TransformApply();
}

void DisplayObject::TranslateY(float value)
{
	this->position_.y += value;
	_TransformApply();
}

void DisplayObject::SetSize(float width, float height)
{
	SetSize(Size{ width, height });
}

void DisplayObject::SetSize(Size size)
{
	if (localSize_.width == 0 || localSize_.height == 0)
	{
		return;
	}
	scale_ = Size{ size.width / localSize_.width, size.height / localSize_.height };
	_TransformApply();
}

void DisplayObject::SetWidth(float value)
{
	if (localSize_.width == 0)
	{
		return;
	}
	scale_.width = value / scale_.width;
	_TransformApply();
}

void DisplayObject::SetHeight(float value)
{
	if (localSize_.height == 0)
	{
		return;
	}
	scale_.height = value / scale_.height;
	_TransformApply();
}

void DisplayObject::SetScale(float scaleX, float scaleY)
{
	SetScale(Size{ scaleX, scaleY });
}

void DisplayObject::SetScale(Size scale)
{
	this->scale_ = scale;
	_TransformApply();
}

void DisplayObject::SetScaleX(float value)
{
	this->scale_.width = value;
	_TransformApply();
}

void DisplayObject::SetScaleY(float value)
{
	this->scale_.height = value;
	_TransformApply();
}

void DisplayObject::SetRotation(float rotation)
{
	this->rotation_ = rotation;
	_TransformApply();
}

void DisplayObject::SetRotationRad(float rotation)
{
	this->rotation_ = RADIAN_TO_DEGREE(rotation);
	_TransformApply();
}

void DisplayObject::SetAnchorPoint(float x, float y)
{
	SetAnchorPoint(Point{ x, y });
}

void DisplayObject::SetAnchorPoint(Point point)
{
	anchorPoint_ = point;
	_TransformApply();
}

void DisplayObject::SetAlpha(float alpha)
{
	this->alpha_ = Clamp<float>(alpha, 0, 1);
}

void DisplayObject::SetVisible(bool visibility)
{
	visible_ = visibility;
}

void DisplayObject::SetDrawOutline(bool draw)
{
	drawOutline_ = draw;
}

DisplayObjectContainer* DisplayObject::GetWorld()
{
	return DisplayObject::world;
}

Camera* DisplayObject::GetCamera() const
{
	return camera_;
}

Point DisplayObject::GetPosition() const
{
	return position_;
}

float DisplayObject::GetX() const
{
	return position_.x;
}

float DisplayObject::GetY() const
{
	return position_.y;
}

Size DisplayObject::GetSize() const
{
	return Size{ localSize_.width * scale_.width, localSize_.height * scale_.height };
}

float DisplayObject::GetWidth() const
{
	return localSize_.width * scale_.width;
}

float DisplayObject::GetHeight() const
{
	return localSize_.height * scale_.height;
}

const Size& DisplayObject::GetLocalSize() const
{
	return localSize_;
}

const Point& DisplayObject::GetLocalPosition() const
{
	return localPoint_;
}

const Size& DisplayObject::GetScale() const
{
	return scale_;
}

float DisplayObject::GetScaleX() const
{
	return scale_.width;
}

float DisplayObject::GetScaleY() const
{
	return scale_.height;
}

float DisplayObject::GetRotation() const
{
	return rotation_;
}

float DisplayObject::GetRotationRad() const
{
	return DEGREE_TO_RADIAN(rotation_);
}

const Point& DisplayObject::GetAnchorPoint() const
{
	return anchorPoint_;
}

bool  DisplayObject::IsVisible() const
{
	return visible_;
}

float DisplayObject::GetAlpha() const
{
	return alpha_;
}

float DisplayObject::GetScreenAlpha() const
{
	if (GetParent())
	{
		return GetAlpha() * GetParent()->GetScreenAlpha();
	}
	return GetAlpha();
}

bool DisplayObject::IsOutlineDrawing() const
{
	return drawOutline_;
}

Matrix3x2 DisplayObject::GetScaleMatrix() const
{
	return Matrix3x2::Scale(scale_, anchorPoint_);
}

Matrix3x2 DisplayObject::GetTranslationMatrix() const
{
	return Matrix3x2::Translation(position_.x, position_.y);
}

Matrix3x2 DisplayObject::GetRotationMatrix() const
{
	return Matrix3x2::Rotation(rotation_, anchorPoint_);
}

Matrix3x2 DisplayObject::DisplayObject::GetCameraMatrix() const
{
	if (camera_)
	{
		Matrix3x2 inverted = camera_->GetMatrix();
		inverted.Invert();
		return inverted;
	}
	return Matrix3x2::Identity();
}

Matrix3x2 DisplayObject::GetMatrix() const
{
	return GetScaleMatrix() * GetRotationMatrix() * GetTranslationMatrix();
}

Matrix3x2 DisplayObject::GetWorldMatrix() const
{
	Matrix3x2 invertedMatrix = world->GetMatrix() * world->GetCameraMatrix();
	invertedMatrix.Invert();
	return GetScreenMatrix() * invertedMatrix;
}

Matrix3x2 DisplayObject::GetScreenMatrix() const
{
	if (GetParent())
	{
		return GetMatrix() * GetCameraMatrix() * GetParent()->GetScreenMatrix();
	}
	return GetMatrix() * GetCameraMatrix();
}

DisplayObjectContainer* DisplayObject::GetParent() const
{
	return parentObject_;
}

Point DisplayObject::WorldToLocal(Point point) const
{
	Matrix3x2 invertWorldMatrix = GetWorldMatrix();
	invertWorldMatrix.Invert();
	return invertWorldMatrix.TransformPoint(point);
}

Point DisplayObject::LocalToWorld(Point point) const
{
	return GetWorldMatrix().TransformPoint(point);
}

Point DisplayObject::ScreenToLocal(Point point) const
{
	Matrix3x2 invertScreenMatrix = GetScreenMatrix();
	invertScreenMatrix.Invert();
	return invertScreenMatrix.TransformPoint(point);
}

Point DisplayObject::LocalToScreen(Point point) const
{
	return GetScreenMatrix().TransformPoint(point);
}

void DisplayObject::_TransformApply()
{
	if (GetParent())
	{
		GetParent()->_UpdateSize();
		GetParent()->_TransformApply();
	}
}

void DisplayObject::Update(double deltaTime)
{
	DispatchEvent(new ObjectEvent(ObjectEvent::UPDATE, this, deltaTime));
}

void DisplayObject::Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DispatchEvent(new ObjectEvent(ObjectEvent::RENDER, this, deltaTime));
	if (IsOutlineDrawing())
	{
		DrawOutline(renderTarget);
	}
}

bool DisplayObject::_RegisterParent(DisplayObjectContainer* parent)
{
	parentObject_ = parent;
	DispatchEvent(new ObjectEvent(ObjectEvent::ADDED, this));
	return true;
}

bool DisplayObject::_UnregisterParent()
{
	parentObject_ = nullptr;
	DispatchEvent(new ObjectEvent(ObjectEvent::REMOVED, this));
	return true;
}

void DisplayObject::DrawOutline(ID2D1HwndRenderTarget* renderTarget)
{
	if (!brush)
	{
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
	}

	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	renderTarget->SetTransform(GetScreenMatrix());
	renderTarget->DrawRectangle(Rect{ GetLocalPosition().x, GetLocalPosition().y, GetLocalPosition().x + GetLocalSize().width, GetLocalPosition().y + GetLocalSize().height }, brush);
	renderTarget->SetTransform(GetScreenMatrix());
	renderTarget->DrawEllipse(D2D1::Ellipse(Point{ 0, 0 }, 2, 2), brush);
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	renderTarget->DrawEllipse(D2D1::Ellipse(GetAnchorPoint(), 1.7f, 1.7f), brush);
}