#pragma once
#include "Resource.h"
#include "ComaRenderer.h"
#include <wincodec.h>
COMA_NS_BEGIN

class Bitmap :
	public Resource
{
private:
	Bitmap(ComaRenderer* renderer, ID2D1Bitmap* bitmap) :renderer(renderer), bitmap(bitmap)
	{
		
	}

public:
	virtual ~Bitmap();
	static Bitmap* createBitmap(ComaRenderer* renderer, TCHAR* filename);
	static Bitmap* createBitmap(ComaRenderer* renderer, IStream* stream);
private:
	static ID2D1Bitmap* createID2D1BitmapFromFile(ID2D1HwndRenderTarget* renderTarget, TCHAR* filename);
	static ID2D1Bitmap* createID2D1BitmapFromStream(ID2D1HwndRenderTarget* renderTarget, IStream* stream);
	static ID2D1Bitmap* createID2D1BitmapFromDecoder(ID2D1HwndRenderTarget* renderTarget, IWICBitmapDecoder* decoder);
	static IWICImagingFactory* factory;
private:
	ComaRenderer* renderer;
	ID2D1Bitmap* bitmap;
};

COMA_END