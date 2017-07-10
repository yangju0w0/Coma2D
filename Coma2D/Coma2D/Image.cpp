/*
* filename	Image.cpp
* fileinfo	이미지 표시객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Image.h"

COMA_USING_NS

Image::Image(Bitmap* bitmap) : bitmap(bitmap), sizeFixed(false)
{
	if (bitmap->isLoaded())
	{
		setLocalSize(bitmap->getResource()->GetSize());
		sizeFixed = true;
	}
}
Image::Image(Bitmap* bitmap, Size size) : bitmap(bitmap), sizeFixed(true)
{
	setLocalSize(size.width, size.height);
}
Image::Image(Bitmap* bitmap, float width, float height) : bitmap(bitmap), sizeFixed(true)
{
	setLocalSize(width, height);
}



Image::~Image()
{
	
}
void Image::render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::render(renderTarget, deltaTime);
	if (!isVisible())
		return;
	if (bitmap->isLoaded())
	{
		if (!sizeFixed)
		{
			setLocalSize(bitmap->getResource()->GetSize());
			sizeFixed = true;
		}
		renderTarget->SetTransform(getScreenMatrix());
		D2D1_RECT_F size = { 0, 0, getLocalSize().width, getLocalSize().height };
		renderTarget->DrawBitmap(bitmap->getResource(), size, getScreenAlpha(), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	}
}