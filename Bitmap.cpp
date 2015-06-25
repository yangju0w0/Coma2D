#include "stdafx.h"
#include "Bitmap.h"
#include "RendererEvent.h"
COMA_USING_NS

Bitmap::~Bitmap()
{
	if(bitmap) bitmap->Release();
}

Bitmap* Bitmap::createBitmap(ComaRenderer* renderer, TCHAR* filename){
	if (!renderer->getRenderTarget())
		return nullptr;

	ID2D1Bitmap* bitmap = createID2D1BitmapFromFile(renderer->getRenderTarget(), filename);

	if (!bitmap)
		return nullptr;

	return new Bitmap(renderer, bitmap);
}
Bitmap* Bitmap::createBitmap(ComaRenderer* renderer, IStream* stream){
	if (!renderer->getRenderTarget())
		return nullptr;

	ID2D1Bitmap* bitmap = createID2D1BitmapFromStream(renderer->getRenderTarget(), stream);

	if (!bitmap)
		return nullptr;

	return new Bitmap(renderer, bitmap);
}

ID2D1Bitmap* Bitmap::createID2D1BitmapFromFile(ID2D1HwndRenderTarget* renderTarget, TCHAR* filename)
{
	if (!renderTarget || !filename)
			return nullptr;

	HRESULT hr;

	if (!factory)
	{
		CoInitialize(0);
		CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	}

	IWICBitmapDecoder* decoder = 0;
	hr = factory->CreateDecoderFromFilename(filename, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr))
	{
		if (decoder) decoder->Release();
		return nullptr;
	}
	
	ID2D1Bitmap* bitmap = createID2D1BitmapFromDecoder(renderTarget, decoder);
	if (decoder) decoder->Release();

	return bitmap;
}
ID2D1Bitmap* Bitmap::createID2D1BitmapFromStream(ID2D1HwndRenderTarget* renderTarget, IStream* stream)
{
	if (!renderTarget || !stream)
		return nullptr;

	HRESULT hr;

	if (!factory)
	{
		CoInitialize(0);
		CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	}

	IWICBitmapDecoder* decoder = 0;
	hr = factory->CreateDecoderFromStream(stream, 0, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr))
	{
		if (decoder) decoder->Release();
		return nullptr;
	}

	ID2D1Bitmap* bitmap = createID2D1BitmapFromDecoder(renderTarget, decoder);
	if (decoder) decoder->Release();

	return bitmap;
}
ID2D1Bitmap* Bitmap::createID2D1BitmapFromDecoder(ID2D1HwndRenderTarget* renderTarget, IWICBitmapDecoder* decoder)
{
	if (!renderTarget || !decoder)
		return nullptr;

	HRESULT hr;

	IWICBitmapFrameDecode* frameDecode = 0;
	hr = decoder->GetFrame(0, &frameDecode);
	if (FAILED(hr))
	{
		if (frameDecode)	frameDecode->Release();
		return nullptr;
	}


	IWICFormatConverter* converter = 0;
	hr = factory->CreateFormatConverter(&converter);
	if (FAILED(hr))
	{
		if (frameDecode)	frameDecode->Release();
		if (converter)		converter->Release();
		return nullptr;
	}


	hr = converter->Initialize(frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0.0, WICBitmapPaletteTypeCustom);
	if (FAILED(hr))
	{
		if (frameDecode) frameDecode->Release();
		if (converter)	converter->Release();
		return nullptr;
	}


	ID2D1Bitmap* bitmap;
	hr = renderTarget->CreateBitmapFromWicBitmap(converter, 0, &bitmap);

	if (FAILED(hr) && bitmap) bitmap->Release();
	if (frameDecode)	frameDecode->Release();
	if (converter)		converter->Release();

	return bitmap;
}