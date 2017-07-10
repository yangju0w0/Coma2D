/*
* filename	Camera.cpp
* fileinfo	DisplayObject용 카메라 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "DisplayObject.h"
#include "Camera.h"
COMA_USING_NS

Camera::Camera(float width, float height)
:DisplayObject(), ref(0), cameraOn(false)
{
	setLocalSize(width, height);
	setVisible(false);
	setAnchorPoint(width / 2.0f, height / 2.0f);
}


Camera::~Camera()
{
	if(brush)brush->Release();
}

void Camera::update()
{
	
}
void Camera::render(ID2D1HwndRenderTarget* renderTarget)
{
	if (!isVisible())
		return;
	if (!brush)
	{
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.8f, 0.1f, 0.1f, getAlpha()), &brush);
	}
	renderTarget->SetTransform(getScreenMatrix());
	renderTarget->DrawRectangle(Rect{ 0, 0, getLocalSize().width, getLocalSize().height }, brush);
}

void Camera::on(){ cameraOn = true; }
void Camera::off(){ cameraOn = false; }
bool Camera::isCameraOn(){ return cameraOn; }

Matrix3x2 Camera::getMatrix()
{
	if (isCameraOn())
	{
		return DisplayObject::getMatrix();
	}
	return Matrix3x2::Identity();
}
bool Camera::_registerParent(DisplayObjectContainer* parent) {
	if (!getParent() && parent)
	{
		if (!DisplayObject::_registerParent(parent))
			return false;
		ref++;
	}
	return true;
}
bool Camera::_unregisterParent()
{
	ref--;
	if (ref <= 0)
	{
		if (!DisplayObject::_unregisterParent())
			return false;
	}
	return true;
}