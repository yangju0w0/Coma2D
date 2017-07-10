/*
* filename	DisplayObjectContainer.cpp
* fileinfo	화면 표시 객체 컨테이너 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/
#include "InteractiveObject.h"
#include "DisplayObjectContainer.h"

COMA_USING_NS

DisplayObjectContainer::DisplayObjectContainer()
:InteractiveObject()
{
}


DisplayObjectContainer::~DisplayObjectContainer()
{
	for (unsigned int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->_unregisterParent();
	}
}

void DisplayObjectContainer::addChild(DisplayObject* object)
{
	if (object->getParent())
		return;
	for (unsigned int i = 0; i < objectList.size(); i++)
		if (objectList[i] == object) return;
	if (!object->_registerParent(this)) return;
	objectList.push_back(object);
	_resetSize();
}
void DisplayObjectContainer::addChildCenter(DisplayObject* object)
{
	if (object->getParent())
		return;
	for (unsigned int i = 0; i < objectList.size(); i++)
		if (objectList[i] == object) return;
	if (!object->_registerParent(this)) return;
	objectList.push_back(object);
	object->setPosition(object->getWidth() / -2, object->getHeight() / -2);
	_resetSize();
}
void DisplayObjectContainer::removeChild(DisplayObject* object)
{
	for (unsigned int i = 0; i < objectList.size(); i++)
	{
		if (objectList[i] == object)
		{
			if (!object->_unregisterParent())
				return;
			objectList.erase(objectList.begin() + i);
			_resetSize();
			return;
		}
	}
}

void DisplayObjectContainer::_resetSize()
{
	Rect rect{ 10000000, 10000000, -10000000, -10000000 };
	for (unsigned int i = 0; i < objectList.size(); i++)
	{
		Matrix3x2 matrix = objectList[i]->getMatrix() * objectList[i]->getCameraMatrix();

		Point point[4] = {
			matrix.TransformPoint(Point{ objectList[i]->getLocalPosition().x,										objectList[i]->getLocalPosition().y }),
			matrix.TransformPoint(Point{ objectList[i]->getLocalPosition().x + objectList[i]->getLocalSize().width, objectList[i]->getLocalPosition().y }),
			matrix.TransformPoint(Point{ objectList[i]->getLocalPosition().x + objectList[i]->getLocalSize().width, objectList[i]->getLocalPosition().y + objectList[i]->getLocalSize().height }),
			matrix.TransformPoint(Point{ objectList[i]->getLocalPosition().x,										objectList[i]->getLocalPosition().y + objectList[i]->getLocalSize().height })
		};

		for (int i = 0; i < 4; i++)
		{
			if (point[i].x < rect.left)
				rect.left = point[i].x;
			if (point[i].y < rect.top)
				rect.top = point[i].y;
			if (point[i].x > rect.right)
				rect.right = point[i].x;
			if (point[i].y > rect.bottom)
				rect.bottom = point[i].y;
		}
	}
	setLocalSize(rect.right - rect.left,rect.bottom - rect.top);
	setLocalPosition(rect.left, rect.top);
}

void DisplayObjectContainer::render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::render(renderTarget, deltaTime);
	if (!isVisible())
		return;
	for (unsigned int i = 0; i < objectList.size(); i++)
	{

		objectList[i]->setDrawOutline(isOutlineDrawing());
		objectList[i]->render(renderTarget, deltaTime);
	}
}
void DisplayObjectContainer::update(double deltaTime)
{
	DisplayObject::update(deltaTime);
	for (unsigned int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->update(deltaTime);
	}
}


void DisplayObjectContainer::drawOutline(ID2D1HwndRenderTarget* renderTarget)
{
	if (!brush)
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	Point position = localToScreen(getLocalPosition());
	Point position2 = localToScreen(Point{ getLocalPosition().x + getLocalSize().width, getLocalPosition().y + getLocalSize().height });
	renderTarget->DrawRectangle(Rect{ position.x, position.y, position2.x, position2.y }, brush);
	renderTarget->SetTransform(getScreenMatrix());
	renderTarget->DrawEllipse(D2D1::Ellipse(Point{ 0, 0 }, 2, 2), brush);
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	renderTarget->DrawEllipse(D2D1::Ellipse(getAnchorPoint(), 1.7f,1.7f),brush);
}