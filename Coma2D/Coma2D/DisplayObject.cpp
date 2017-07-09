/*
* The MIT License (MIT)
*
* Copyright (c) 2015 Heonyang Ju
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
* MIT 라이선스 (MIT)
*
* 저작권(C) 2015 주헌양
*
* 이 문서에 의해, 아래의 조건을 따르는 한 제한 없이 소프트웨어를 다루기 위해
* 소프트웨어의 복사본과 관련된 문서("소프트웨어")를 획득하는 사람에게 변경의
* 자유가 허용 된다. 제한 없음은 사용, 복제, 수정, 통합, 공포, 배포, 하위 라이선스
* 그리고/또는 소프트웨어의 복사본의 판매와 공급된 소프트웨어를 가진 사람에게
* 그렇게 할 수 있도록 허가하는 권한에 제한 없음을 포함 한다.
*
* 위의 저작권 표시와 이 허가 표시는 모든 소프트웨어의 복사본 또는 상당한 일부분에
* 포함되어야 한다.
*
* 이 소프트웨어는 특정한 목적과 침해 없음을 위해 상업성, 적합성의 보증에만
* 국한되는 것이 아닌 명시적 또는 묵시적인 어떠한 종류의 보증없이 "있는 그대로"
* 제공 된다. 어떠한 경우에도 저작자 또는 저작권자는 소프트웨어의 관계 안과 밖
* 또는 소프트웨어의 사용 또는 거래로부터 발생하는 계약, 불법 행위 또는 기타 수행에
* 대해, 어떠한 소송, 손해 또는 다른 책무에 대해서도 책임이 없다.
*
* 본문은 MIT라이선스 원문의 번역본이며, 모든 해석은 원문에 기초한다.
*/

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