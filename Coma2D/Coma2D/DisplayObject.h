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
	void setLocalSize(Size size) { localSize = size;  _transformApply(); }
	void setLocalSize(float width, float height) { setLocalSize(Size{ width, height }); }
	void setLocalPosition(Point position){ localPoint = position; }
	void setLocalPosition(float x, float y){ setLocalPosition(Point{ x, y }); }

	static ID2D1SolidColorBrush* brush;
private:
	static DisplayObjectContainer* world;	//월드 객체

	DisplayObjectContainer* parentObject;	//부모 객체
	Camera*					camera;			//카메라 객체

	Point	position;		//위치
	Size	scale;			//크기
	float	rotation;		//회전
	Point	anchorPoint;	//기준점
	bool	visible;		//가시 여부
	float	alpha;			//투명도
	Size	localSize;		//로컬 크기
	Point	localPoint;		//로컬 시작 위치

	bool	outlineDrawing;	//경계선 그리기 여부

public:
	virtual ~DisplayObject();

	virtual void update(double deltaTime);										//그리기 작업 전에 호출
	virtual void render(ID2D1HwndRenderTarget* renderTarget, double deltaTime);	//그리기 작업
	
//Getter
	static DisplayObjectContainer* getWorld(){ return DisplayObject::world; }
	Camera* getCamera()		{ return camera; }

	Point	getPosition()	{ return position; }
	float	getX()			{ return position.x; }
	float	getY()			{ return position.y; }
	Size	getSize()		{ return Size{ localSize.width * scale.width, localSize.height * scale.height }; }
	float	getWidth()		{ return localSize.width * scale.width; }
	float	getHeight()		{ return localSize.height * scale.height; }
	Size	getLocalSize()	{ return localSize; }
	Point	getLocalPosition(){ return localPoint; }
	Size	getScale()		{ return scale; }
	float	getScaleX()		{ return scale.width; }
	float	getScaleY()		{ return scale.height; }
	float	getRotation()	{ return rotation; }
	float	getRotationRad(){ return DEGREE_TO_RADIAN(rotation); }
	Point	getAnchorPoint(){ return anchorPoint; }
	bool	isVisible()		{ return visible; }
	float	getAlpha()		{ return alpha; }
	float	getScreenAlpha();
	bool	isOutlineDrawing(){ return outlineDrawing; }

	Matrix3x2 getScaleMatrix()			{ return Matrix3x2::Scale(scale, anchorPoint); }				//화면 비율 행렬을 반환한다.
	Matrix3x2 getTranslationMatrix()	{ return Matrix3x2::Translation(position.x, position.y); }		//이동 행렬을 반환한다.
	Matrix3x2 getRotationMatrix()		{ return Matrix3x2::Rotation(rotation, anchorPoint); }			//회전 행렬을 반환한다.
	Matrix3x2 getCameraMatrix();																		//카메라의 변환 행렬을 반환한다.
	Matrix3x2 getMatrix()				{ return getScaleMatrix()* getRotationMatrix() *getTranslationMatrix(); }	//해당 Object의 변환 행렬을 반환한다.
	Matrix3x2 getWorldMatrix();		//월드 오브젝트 내에서의 행렬 변환을 반환한다. 주의: 월드에 포함되지 않은 경우 정확한 결과가 나오지 않을 수 있음.
	Matrix3x2 getScreenMatrix();	//해당 오브젝트를 포함한 상위 오브젝트의 행렬의 곱을 반환한다. (재귀)

	DisplayObjectContainer* getParent()						{ return parentObject; }	//부모 객체를 반환한다.
	virtual bool _registerParent(DisplayObjectContainer* parent);	//부모 객체를 등록한다. (외부호출 금지)
	virtual bool _unregisterParent();								//부모 객체를 해제한다. (외부호출 금지)

	

//Setter
	static void _setWorld(DisplayObjectContainer* world);
	void setCamera(Camera* camera);	//카메라 설정
	void unsetCamera();				//카메라 해제

	void setPosition(float x, float y)		{ setPosition(Point{ x, y }); }
	void setPosition(Point position)		{ this->position = position;	_transformApply(); }
	void setX(float value)					{ this->position.x = value;		_transformApply(); }
	void setY(float value)					{ this->position.y = value;		_transformApply(); }
	void translate(Point vector)			{ this->position.x += vector.x; this->position.y += vector.y;  _transformApply(); }
	void translate(float x, float y)		{ translate(D2D1::Point2F(x, y)); }
	void translateX(float value)			{ this->position.x += value;	_transformApply(); }
	void translateY(float value)			{ this->position.y += value;	_transformApply(); }
	void setSize(float width, float height)	{ setSize(Size{ width, height }); }
	void setSize(Size size)					{ if (localSize.width == 0 || localSize.height == 0)return; scale = Size{ size.width / localSize.width, size.height / localSize.height }; _transformApply(); }
	void setWidth(float value)				{ if (localSize.width == 0)return; scale.width = value / scale.width;  _transformApply(); }
	void setHeight(float value)				{ if (localSize.height == 0)return; scale.height = value / scale.height; _transformApply(); }
	void setScale(float scaleX, float scaleY){ setScale(Size{ scaleX, scaleY }); }
	void setScale(Size scale)				{ this->scale = scale;			_transformApply(); }
	void setScaleX(float value)				{ this->scale.width = value;	_transformApply(); }
	void setScaleY(float value)				{ this->scale.height = value;	_transformApply(); }
	void setRotation(float rotation)		{ this->rotation = rotation;	_transformApply(); }
	void setRotationRad(float rotation)		{ this->rotation = RADIAN_TO_DEGREE(rotation); _transformApply(); }
	void setAnchorPoint(float x, float y)	{ setAnchorPoint(Point{ x, y }); }
	void setAnchorPoint(Point point)		{ anchorPoint = point;			_transformApply(); }
	void setAlpha(float alpha)				{ this->alpha = alpha; if (alpha < 0) this->alpha = 0; else if (alpha > 1) this->alpha = 1;}
	void setVisible(bool visibility)		{ visible = visibility; }
	virtual void setDrawOutline(bool draw)			{ outlineDrawing = draw; }
//World좌표계를 기준으로 위치를 변환한다.
	Point worldToLocal(Point point){
		Matrix3x2 invertWorldMatrix = getWorldMatrix();
		invertWorldMatrix.Invert();
		return invertWorldMatrix.TransformPoint(point);
	}
	Point localToWorld(Point point){ 
		return getWorldMatrix().TransformPoint(point);
	}
//윈도우 화면의 좌표계를 기준으로 위치를 변환한다.
	Point screenToLocal(Point point){
		Matrix3x2 invertScreenMatrix = getScreenMatrix();
		invertScreenMatrix.Invert();
		return invertScreenMatrix.TransformPoint(point);
	}
	Point localToScreen(Point point){
		return getScreenMatrix().TransformPoint(point);
	}

	virtual void _transformApply();	//변환을 부모 객체에게 알릴 때 호출한다.(외부호출 금지)
protected:
	virtual void drawOutline(ID2D1HwndRenderTarget* renderTarget);
};

COMA_END