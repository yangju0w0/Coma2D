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
* filename	DisplayObject.h
* fileinfo	화면 표시 객체 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "EventDispatcher.h"
#include <math.h>
#include <d2d1.h>

COMA_NS_BEGIN

class DisplayObjectContainer;
class Camera;

class DisplayObject :
	public EventDispatcher
{
protected:
	DisplayObject();
private:
	static DisplayObjectContainer* world;
public:
	virtual ~DisplayObject();

	virtual void update(){}
	virtual void render(ID2D1HwndRenderTarget* renderTarget){}
	
//Getter
	Camera* getCamera()		{ return camera; }
	DisplayObjectContainer* getWorld(){ return world; }
	Point getPosition()		{ return position; }
	float getX()			{ return position.x; }
	float getY()			{ return position.y; }
	Size getLocalSize()		{ return localSize; }

	Size getSize()			{ return Size{ localSize.width * scale.width, localSize.height * scale.height }; }
	float getWidth()		{ return localSize.width * scale.width; }
	float getHeight()		{ return localSize.height * scale.height; }

	Size getScale()			{ return scale; }
	float getScaleX()		{ return scale.width; }
	float getScaleY()		{ return scale.height; }

	float getRotation()		{ return rotation; }
	float getRotationRad()	{ return DEGREE_TO_RADIAN(rotation); }

	Point getAnchorPoint()	{ return anchorPoint; }

	Matrix3x2 getScaleMatrix()			{ return Matrix3x2::Scale(scale, anchorPoint); }				//화면 비율 행렬을 반환합니다.
	Matrix3x2 getTranslationMatrix()	{ return Matrix3x2::Translation(position.x, position.y); }		//이동 행렬을 반환합니다.
	Matrix3x2 getRotationMatrix()		{ return Matrix3x2::Rotation(rotation, anchorPoint); }			//회전 행렬을 반환합니다.
	Matrix3x2 getCameraMatrix();																		//카메라의 변환 행렬을 반환합니다.
	Matrix3x2 getMatrix()				{ return getScaleMatrix() * getTranslationMatrix() * getRotationMatrix(); }	//해당 Object의 변환 행렬을 반환합니다.
	Matrix3x2 getWorldMatrix();
	Matrix3x2 getScreenMatrix();	//해당 오브젝트를 포함한 상위 오브젝트의 행렬의 곱을 반환합니다.                                                                      재귀)

	DisplayObjectContainer* getParentObject()	{ return parentObject; }
	void _registerParent(DisplayObjectContainer* parent) { parentObject = parent; }
	void _unregisterParent() { parentObject = nullptr; }

	bool isVisible(){ return visible; }
	float getAlpha(){ return alpha; }

//Setter
	void setCamera(Camera* camera);
	void setWorld(DisplayObjectContainer* world);
	void unsetCamera();
	void setPosition(float x, float y)	{ setPosition(Point{x, y}); }
	void setPosition(Point position)	{ this->position = position; }
	void setX(float value)				{ this->position.x = value; }
	void setY(float value)				{ this->position.y = value; }
	void translate(Point vector)		{ this->position.x += vector.x; this->position.y += vector.y; }
	void translate(float x, float y)	{ translate(D2D1::Point2F(x, y)); }
	void translateX(float value)		{ this->position.x += value; }
	void translateY(float value)		{ this->position.y += value; }

	void setSize(float width, float height){ setSize(Size{ width, height }); }
	void setSize(Size size)		{ if (localSize.width == 0 || localSize.height == 0)return; scale = Size{ size.width / localSize.width, size.height / localSize.height }; }
	void setWidth(float value)	{ if (localSize.width == 0)return; scale.width = value / scale.width; }
	void setHeight(float value)	{ if (localSize.height == 0)return; scale.height = value / scale.height; }
	
	void setScale(float scaleX, float scaleY){ setScale(Size{ scaleX, scaleY });}
	void setScale(Size scale)		{ this->scale = scale; }
	void setScaleX(float value)		{ this->scale.width = value; }
	void setScaleY(float value)		{ this->scale.height = value; }

	void setRotation(float rotation){ this->rotation = rotation; }
	void setRotationRad(float rotation){ this->rotation = RADIAN_TO_DEGREE(rotation); }
	
	void setAnchorPoint(float x, float y){ setAnchorPoint(Point{ x, y }); }
	void setAnchorPoint(Point point){ anchorPoint = point; }

	void setAlpha(float alpha)
	{
		this->alpha = alpha;
		if (alpha < 0) this->alpha = 0;
		else if (alpha > 1) this->alpha = 1;
	}
	void setVisible(bool visibility){ visible = visibility; }


	Point worldToLocal(Point point){ return getWorldMatrix().TransformPoint(point); }
	Point localToWorld(Point point){ 
		Matrix3x2 invertWorldMatrix = getWorldMatrix();
		invertWorldMatrix.Invert();
		return invertWorldMatrix.TransformPoint(point);
	}
	Point screenToLocal(Point point){ return getScreenMatrix().TransformPoint(point); }
	Point localToScreen(Point point){
		Matrix3x2 invertScreenMatrix = getScreenMatrix();
		invertScreenMatrix.Invert();
		return invertScreenMatrix.TransformPoint(point);
	}
	
protected:
	void setLocalSize(Size size) { localSize = size; }
	void setLocalSize(float width, float height) { setLocalSize(Size{ width, height }); }

private:
	DisplayObjectContainer* parentObject;
	Camera* camera;
	Point position;
	Size scale;
	float rotation;
	Point anchorPoint;

	bool visible;
	float alpha;

	Size localSize;
};

COMA_END