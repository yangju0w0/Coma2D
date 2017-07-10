/*
* filename	Image.h
* fileinfo	이미지 표시객체 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "DisplayObject.h"
#include "Bitmap.h"

COMA_NS_BEGIN

class Image :
	public DisplayObject
{
public:
	explicit Image(Bitmap* bitmap);
	Image(Bitmap* bitmap, Size size);
	Image(Bitmap* bitmap, float width, float height);
	virtual ~Image();
	virtual void render(ID2D1HwndRenderTarget* renderTarget, double deltaTime);
	Bitmap* getBitmap(){ return bitmap; }
protected:
	bool sizeFixed;
	Bitmap* bitmap;
};

COMA_END