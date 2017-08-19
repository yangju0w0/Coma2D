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
	for (const auto& obj : objectList_)
	{
		obj->_UnregisterParent();
	}
}

void DisplayObjectContainer::AddChild(DisplayObject* object)
{
	if (object->GetParent())
	{
		return;
	}

	for (const auto& obj : objectList_)
	{
		if (obj == object)
		{
			return;
		}
	}

	if (!object->_RegisterParent(this))
	{
		return;
	}

	objectList_.push_back(object);
	_UpdateSize();
}

void DisplayObjectContainer::AddChildCenter(DisplayObject* object)
{
	if (object->GetParent())
	{
		return;
	}

	for (const auto& obj : objectList_)
	{
		if (obj == object)
		{
			return;
		}
	}

	if (!object->_RegisterParent(this))
	{
		return;
	}

	objectList_.push_back(object);
	object->SetPosition(object->GetWidth() / -2, object->GetHeight() / -2);
	_UpdateSize();
}

void DisplayObjectContainer::RemoveChild(DisplayObject* object)
{
	for (auto iter = objectList_.begin(); iter != objectList_.end(); ++iter)
	{
		if ((*iter) == object)
		{
			if (!object->_UnregisterParent())
			{
				return;
			}
			objectList_.erase(iter);
			_UpdateSize();
			return;
		}
	}
}

void DisplayObjectContainer::_UpdateSize()
{
	Rect rect{ FLT_MAX, FLT_MAX, FLT_MIN, FLT_MIN };
	for (unsigned int i = 0; i < objectList_.size(); i++)
	{
		Matrix3x2 matrix = objectList_[i]->GetMatrix() * objectList_[i]->GetCameraMatrix();

		Point point[4] = {
			matrix.TransformPoint(Point{ objectList_[i]->GetLocalPosition().x, objectList_[i]->GetLocalPosition().y }),
			matrix.TransformPoint(Point{ objectList_[i]->GetLocalPosition().x + objectList_[i]->GetLocalSize().width, objectList_[i]->GetLocalPosition().y }),
			matrix.TransformPoint(Point{ objectList_[i]->GetLocalPosition().x + objectList_[i]->GetLocalSize().width, objectList_[i]->GetLocalPosition().y + objectList_[i]->GetLocalSize().height }),
			matrix.TransformPoint(Point{ objectList_[i]->GetLocalPosition().x, objectList_[i]->GetLocalPosition().y + objectList_[i]->GetLocalSize().height })
		};

		for (int i = 0; i < 4; i++)
		{
			if (point[i].x < rect.left)
			{
				rect.left = point[i].x;
			}
			if (point[i].y < rect.top)
			{
				rect.top = point[i].y;
			}	
			if (point[i].x > rect.right)
			{
				rect.right = point[i].x;
			}	
			if (point[i].y > rect.bottom)
			{
				rect.bottom = point[i].y;
			}
		}
	}
	SetLocalSize(rect.right - rect.left,rect.bottom - rect.top);
	SetLocalPosition(rect.left, rect.top);
}

void DisplayObjectContainer::Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::Render(renderTarget, deltaTime);
	if (!IsVisible())
	{
		return;
	}

	for (const auto& obj : objectList_)
	{
		obj->SetDrawOutline(IsOutlineDrawing());
		obj->Render(renderTarget, deltaTime);
	}
}

void DisplayObjectContainer::Update(double deltaTime)
{
	DisplayObject::Update(deltaTime);
	for (const auto& obj : objectList_)
	{
		obj->Update(deltaTime);
	}
}

void DisplayObjectContainer::DrawOutline(ID2D1HwndRenderTarget* renderTarget)
{
	if (!brush)
	{
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush);
	}
		
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	Point position = LocalToScreen(GetLocalPosition());
	Point position2 = LocalToScreen(Point{ GetLocalPosition().x + GetLocalSize().width, GetLocalPosition().y + GetLocalSize().height });
	renderTarget->DrawRectangle(Rect{ position.x, position.y, position2.x, position2.y }, brush);
	renderTarget->SetTransform(GetScreenMatrix());
	renderTarget->DrawEllipse(D2D1::Ellipse(Point{ 0, 0 }, 2, 2), brush);
	brush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	renderTarget->DrawEllipse(D2D1::Ellipse(GetAnchorPoint(), 1.7f,1.7f),brush);
}