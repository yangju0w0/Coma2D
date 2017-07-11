/*
* filename	Bitmap.h
* fileinfo	비트맵 리소스 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Bitmap.h"
#include "RendererEvent.h"
COMA_USING_NS

Bitmap::~Bitmap()
{
	if (renderer)
	{
		renderer->RemoveEventListener(RendererEvent::LOAD_RESOURCE_REQ,		this);
		renderer->RemoveEventListener(RendererEvent::UNLOAD_RESOURCES_REQ,	this);
	}
	if(decoder)decoder->Release();
	if(bitmap && isLoaded()) bitmap->Release();
}
IWICImagingFactory* Bitmap::factory = nullptr;
ComaRenderer* Bitmap::mainRenderer = nullptr;

Bitmap* Bitmap::createBitmap(TCHAR* filename)
{
	return createBitmap(mainRenderer, filename);
}
Bitmap* Bitmap::createBitmap(IStream* stream)
{
	return createBitmap(mainRenderer, stream);
}
Bitmap* Bitmap::createBitmap(ComaRenderer* renderer, TCHAR* filename){
	if (!renderer->getRenderTarget())
		return nullptr;

	IWICBitmapDecoder* decoder = createBitmapDecoderFromFile(filename);
	if (!decoder)
		return nullptr;

	return new Bitmap(renderer, decoder);
}
Bitmap* Bitmap::createBitmap(ComaRenderer* renderer, IStream* stream){
	if (!renderer->getRenderTarget())
		return nullptr;

	IWICBitmapDecoder* decoder = createBitmapDecoderFromStream(stream);
	if (!decoder)
		return nullptr;

	return new Bitmap(renderer, decoder);
}

Bitmap* Bitmap::createBitmapAndLoad(TCHAR* filename)
{
	return createBitmapAndLoad(mainRenderer, filename);
}
Bitmap* Bitmap::createBitmapAndLoad(IStream* stream)
{
	return createBitmapAndLoad(mainRenderer, stream);
}
Bitmap* Bitmap::createBitmapAndLoad(ComaRenderer* renderer, TCHAR* filename){
	if (!renderer->getRenderTarget())
		return nullptr;

	IWICBitmapDecoder* decoder = createBitmapDecoderFromFile(filename);
	if (!decoder)
		return nullptr;

	return new Bitmap(renderer, decoder, true);
}
Bitmap* Bitmap::createBitmapAndLoad(ComaRenderer* renderer, IStream* stream){
	if (!renderer->getRenderTarget())
		return nullptr;

	IWICBitmapDecoder* decoder = createBitmapDecoderFromStream(stream);
	if (!decoder)
		return nullptr;

	return new Bitmap(renderer, decoder, true);
}

void Bitmap::setRenderer(ComaRenderer* renderer)
{
	mainRenderer = renderer;
} 

IWICBitmapDecoder* Bitmap::createBitmapDecoderFromFile(TCHAR* filename)
{
	if (!filename)
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
	return decoder;
}
IWICBitmapDecoder* Bitmap::createBitmapDecoderFromStream(IStream* stream)
{
	if (!stream)
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

	return decoder;
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

void Bitmap::loadReqListener(const Event* event)
{
	if (!reload)
		return;
	//reload = false;
	//loadResource();
}
void Bitmap::unloadReqListener(const Event* event)
{
	if (!isLoaded())
		return;
	//reload = true;
	//unloadResource();
}

bool Bitmap::loadResource()
{
	if (!isLoaded())
	{
		bitmap = createID2D1BitmapFromDecoder(renderer->getRenderTarget(), decoder);
		return isLoaded();
	}
	return true;
}
bool Bitmap::unloadResource()
{
	if (isLoaded())
	{
		if (bitmap) bitmap->Release();
		bitmap = nullptr;
		return !isLoaded();
	}
	return true;
}
bool Bitmap::isLoaded()
{
	if (bitmap)
		return true;
	return false;
}