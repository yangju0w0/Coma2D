#pragma once
#include "DisplayObject.h"
#include "Bitmap.h"

COMA_NS_BEGIN

class Image :
	public DisplayObject
{
public:
	Image(Bitmap* bitmap);
	Image(Bitmap* bitmap, Size size);
	Image(Bitmap* bitmap, float width, float height);
	virtual ~Image();
	void update();
	void render(ID2D1HwndRenderTarget* renderTarget);
private:
	bool sizeFixed;
	Bitmap* bitmap;
};

COMA_END