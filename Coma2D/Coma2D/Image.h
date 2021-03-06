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
	virtual void Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime);
	Bitmap* GetBitmap() { return bitmap_; }
protected:
	bool sizeFixed_;
	Bitmap* bitmap_;
};

COMA_END