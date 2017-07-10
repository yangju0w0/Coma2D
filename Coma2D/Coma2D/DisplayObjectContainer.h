/*
* filename	DisplayObjectContainer.h
* fileinfo	화면 표시 객체 컨테이너 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "InteractiveObject.h"
#include <vector>

COMA_NS_BEGIN

class DisplayObjectContainer :
	public InteractiveObject
{
public:
	DisplayObjectContainer();
	virtual ~DisplayObjectContainer();

	virtual void update(double deltaTime);
	virtual void render(ID2D1HwndRenderTarget* renderTarget, double deltaTime); // 자식 객체를 모두 그린다.

	void addChild(DisplayObject* object);	//자식 객체 추가
	void addChildCenter(DisplayObject* object); // 자식객체 중앙에 추가
	void removeChild(DisplayObject* object);//자식 객체 제거

	void _resetSize();				//자식 객체의 크기와 위치에 맞게 사이즈를 다시 조정한다.
private:
	std::vector<DisplayObject*> objectList;
protected:
	virtual void drawOutline(ID2D1HwndRenderTarget* renderTarget);
};

COMA_END