#include "stdafx.h"
#include "Bitmap.h"
using namespace coma2d;

IWICImagingFactory* Bitmap::factory = 0;
const std::string Bitmap::TYPE_BITMAP = "bitmap";

bool Bitmap::loadResource()
{
	if (filename)
	{
		bitmap = CreateBitmapFromFile(filename);
	}
	else if (stream)
	{
		bitmap = CreateBitmapFromStream(stream);
	}
	if (!bitmap)
		return false;
	return true;
}
bool Bitmap::unloadResource()
{
	if (bitmap)
	{
		bitmap->Release();
		return true;
	}
	return false;
}
ID2D1Bitmap* Bitmap::getResource()
{
	return bitmap;
}