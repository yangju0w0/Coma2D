/*
* filename	DisplayObject.cpp
* fileinfo	화면 표시 객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "DisplayObject.h"
#include "DisplayObjectContainer.h"
#include "Camera.h"
#include "ObjectEvent.h"
COMA_USING_NS

DisplayObjectContainer* DisplayObject::world = nullptr;
ID2D1SolidColorBrush* DisplayObject::brush = nullptr;

DisplayObject::DisplayObject()
	:position(Point{ 0, 0 }), scale(Size{ 1, 1 }), rotation(0), anchorPoint(Point{ 0, 0 }), visible(true), alpha(1.0f), localSize(Size{ 0, 0 }), localPoint(Point{ 0, 0 }), outlineDrawing(false)
{
}


DisplayObject::~DisplayObject()
{
	if (getParent())
	{ 
		getParent()->removeChild(this);
	}
	if (outlineDrawing)
	{
		if (brush)
		{
			brush->Release();
			brush = nullptr;
		}
	}
}

Matrix3x2 DisplayObject::getCameraMatrix()
{
	if (camera)
	{
		Matrix3x2 inverted = camera->getMatrix();
		inverted.Invert();
		return inverted;
	}
	return Matrix3x2::Identity();
}

void DisplayObject::setCamera(Camera* camera)
{
	if (camera)
	{
		this->camera = camera;
		camera->_registerParent(parentObject);
	}
}
void DisplayObject::unsetCamera()
{
	if (this->camera)
	{
		camera->_unregisterParent();
		camera = nullptr;
	}
}
void DisplayObject::_setWorld(DisplayObjectContainer* world)
{
	DisplayObject::world = world;
}

Matrix3x2 DisplayObject::getScreenMatrix()
{
	if (getParent())
		return getMatrix() * getCameraMatrix()* getParent()->getScreenMatrix();
	return getMatrix()*getCameraMatrix();
}
Matrix3x2 DisplayObject::getWorldMatrix()
{
	Matrix3x2 invertedMatrix = world->getMatrix() * world->getCameraMatrix();
	invertedMatrix.Invert();
	return getScreenMatrix()* invertedMatrix;
}

float DisplayObject::getScreenAlpha()
{
	if (getParent())
		return getAlpha() * getParent()->getScreenAlpha();
	return getAlpha();
}

void DisplayObject::_transformApply()
{
	if (getParent())
	{
		getParent()->_resetSize();
		getParent()->_transformApply();
	}
}

void DisplayObject::update(double deltaTime)
{
	dispatchEvent(new ObjectEvent(ObjectEvent::UPDATE, this, deltaTime));
}
void DisplayObject::render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	dispatchEvent(new ObjectEvent(ObjectEvent::RENDER, this, deltaTime));
	if (isOutlineDrawing())
		drawOutline(renderTarget);
}

bool DisplayObject::_registerParent(DisplayObjectContainer* parent)
{ 
	parentObject = parent;
	dispatchEvent(new ObjectEvent(ObjectEvent::ADDED, this));
	return true;
}	
bool DisplayObject::_unregisterParent()
{
	parentObject = nullptr;
	dispatchEvent(new ObjectEvent(ObjectEvent::REMOVED, this));
	return true;
}

void DisplayObject::drawOutline(ID2D1HwndRenderTarget* renderTarget)
{
	if (!brush)
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	renderTarget->SetTransform(getScreenMatrix());
	renderTarget->DrawRectangle(Rect{ getLocalPosition().x, getLocalPosition().y, getLocalPosition().x + getLocalSize().width, getLocalPosition().y + getLocalSize().height }, brush);
	renderTarget->SetTransform(getScreenMatrix());
	renderTarget->DrawEllipse(D2D1::Ellipse(Point{ 0, 0 }, 2, 2), brush);
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	renderTarget->DrawEllipse(D2D1::Ellipse(getAnchorPoint(), 1.7f, 1.7f), brush);
}