/*
* filename	Camera.cpp
* fileinfo	DisplayObject용 카메라 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "DisplayObject.h"
#include "Camera.h"
COMA_USING_NS

Camera::Camera(float width, float height)
	:DisplayObject(), ref_(0), cameraOn_(false), cameraBrush_(nullptr)
{
	SetLocalSize(width, height);
	SetVisible(false);
	SetAnchorPoint(width / 2.0f, height / 2.0f);
}


Camera::~Camera()
{
	if (cameraBrush_)
	{
		cameraBrush_->Release();
	}
}

void Camera::Update()
{

}

void Camera::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (!IsVisible())
	{
		return;
	}

	if (!cameraBrush_)
	{
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.8f, 0.1f, 0.1f, GetAlpha()), &cameraBrush_);
	}
	renderTarget->SetTransform(GetScreenMatrix());
	renderTarget->DrawRectangle(Rect{ 0, 0, GetLocalSize().width, GetLocalSize().height }, cameraBrush_);
}

void Camera::On()
{
	cameraOn_ = true;
}

void Camera::Off()
{
	cameraOn_ = false;
}

bool Camera::IsCameraOn() const
{
	return cameraOn_;
}

Matrix3x2 Camera::GetMatrix()
{
	if (IsCameraOn())
	{
		return DisplayObject::GetMatrix();
	}
	return Matrix3x2::Identity();
}

bool Camera::_RegisterParent(DisplayObjectContainer* parent)
{
	if (!GetParent() && parent)
	{
		if (!DisplayObject::_RegisterParent(parent))
		{
			return false;
		}
		ref_++;
	}
	return true;
}

bool Camera::_UnregisterParent()
{
	ref_--;
	if (ref_ <= 0)
	{
		if (!DisplayObject::_UnregisterParent())
		{
			return false;
		}
	}
	return true;
}