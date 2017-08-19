/*
* filename	Image.cpp
* fileinfo	이미지 표시객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Image.h"

COMA_USING_NS

Image::Image(Bitmap* bitmap) : bitmap_(bitmap), sizeFixed_(false)
{
	if (bitmap_->IsLoaded())
	{
		SetLocalSize(bitmap_->GetResource()->GetSize());
		sizeFixed_ = true;
	}
}

Image::Image(Bitmap* bitmap, Size size) : bitmap_(bitmap), sizeFixed_(true)
{
	SetLocalSize(size.width, size.height);
}

Image::Image(Bitmap* bitmap, float width, float height) : bitmap_(bitmap), sizeFixed_(true)
{
	SetLocalSize(width, height);
}

Image::~Image()
{
	
}

void Image::Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::Render(renderTarget, deltaTime);
	if (!IsVisible())
	{
		return;
	}

	if (bitmap_->IsLoaded())
	{
		if (!sizeFixed_)
		{
			SetLocalSize(bitmap_->GetResource()->GetSize());
			sizeFixed_ = true;
		}
		renderTarget->SetTransform(GetScreenMatrix());
		D2D1_RECT_F size = { 0, 0, GetLocalSize().width, GetLocalSize().height };
		renderTarget->DrawBitmap(bitmap_->GetResource(), size, GetScreenAlpha(), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	}
}