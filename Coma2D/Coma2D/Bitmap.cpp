/*
* filename	Bitmap.h
* fileinfo	비트맵 리소스 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Bitmap.h"
#include "RendererEvent.h"
COMA_USING_NS

IWICImagingFactory* Bitmap::factory = nullptr;
ComaRenderer* Bitmap::mainRenderer = nullptr;

Bitmap::~Bitmap()
{
	if (renderer_)
	{
		renderer_->RemoveEventListener(RendererEvent::LOAD_RESOURCE_REQ, this);
		renderer_->RemoveEventListener(RendererEvent::UNLOAD_RESOURCES_REQ, this);
	}
	if (decoder_) decoder_->Release();
	if (bitmap_ && IsLoaded()) bitmap_->Release();
}

Bitmap* Bitmap::CreateBitmap(TCHAR* filename)
{
	return CreateBitmap(mainRenderer, filename);
}

Bitmap* Bitmap::CreateBitmap(IStream* stream)
{
	return CreateBitmap(mainRenderer, stream);
}

Bitmap* Bitmap::CreateBitmap(ComaRenderer* renderer, TCHAR* filename)
{
	if (!renderer->GetRenderTarget())
	{
		return nullptr;
	}

	IWICBitmapDecoder* decoder = CreateBitmapDecoderFromFile(filename);
	if (!decoder)
	{
		return nullptr;
	}

	return new Bitmap(renderer, decoder);
}

Bitmap* Bitmap::CreateBitmap(ComaRenderer* renderer, IStream* stream)
{
	if (!renderer->GetRenderTarget())
	{
		return nullptr;
	}

	IWICBitmapDecoder* decoder = CreateBitmapDecoderFromStream(stream);
	if (!decoder)
	{
		return nullptr;
	}

	return new Bitmap(renderer, decoder);
}

Bitmap* Bitmap::CreateBitmapAndLoad(TCHAR* filename)
{
	return CreateBitmapAndLoad(mainRenderer, filename);
}

Bitmap* Bitmap::CreateBitmapAndLoad(IStream* stream)
{
	return CreateBitmapAndLoad(mainRenderer, stream);
}

Bitmap* Bitmap::CreateBitmapAndLoad(ComaRenderer* renderer, TCHAR* filename)
{
	if (!renderer->GetRenderTarget())
	{
		return nullptr;
	}

	IWICBitmapDecoder* decoder = CreateBitmapDecoderFromFile(filename);
	if (!decoder)
	{
		return nullptr;
	}

	return new Bitmap(renderer, decoder, true);
}

Bitmap* Bitmap::CreateBitmapAndLoad(ComaRenderer* renderer, IStream* stream)
{
	if (!renderer->GetRenderTarget())
	{
		return nullptr;
	}

	IWICBitmapDecoder* decoder = CreateBitmapDecoderFromStream(stream);
	if (!decoder)
	{
		return nullptr;
	}

	return new Bitmap(renderer, decoder, true);
}

void Bitmap::SetRenderer(ComaRenderer* renderer)
{
	mainRenderer = renderer;
}

IWICBitmapDecoder* Bitmap::CreateBitmapDecoderFromFile(TCHAR* filename)
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
IWICBitmapDecoder* Bitmap::CreateBitmapDecoderFromStream(IStream* stream)
{
	if (!stream)
	{
		return nullptr;
	}

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

ID2D1Bitmap* Bitmap::CreateID2D1BitmapFromDecoder(ID2D1HwndRenderTarget* renderTarget, IWICBitmapDecoder* decoder)
{
	if (!renderTarget || !decoder)
	{
		return nullptr;
	}

	HRESULT hr;

	IWICBitmapFrameDecode* frameDecode = 0;
	hr = decoder->GetFrame(0, &frameDecode);
	if (FAILED(hr))
	{
		if (frameDecode) frameDecode->Release();
		return nullptr;
	}


	IWICFormatConverter* converter = 0;
	hr = factory->CreateFormatConverter(&converter);
	if (FAILED(hr))
	{
		if (frameDecode) frameDecode->Release();
		if (converter) converter->Release();
		return nullptr;
	}


	hr = converter->Initialize(frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0.0, WICBitmapPaletteTypeCustom);
	if (FAILED(hr))
	{
		if (frameDecode) frameDecode->Release();
		if (converter) converter->Release();
		return nullptr;
	}

	ID2D1Bitmap* bitmap;
	hr = renderTarget->CreateBitmapFromWicBitmap(converter, 0, &bitmap);

	if (FAILED(hr) && bitmap) bitmap->Release();
	if (frameDecode) frameDecode->Release();
	if (converter) converter->Release();

	return bitmap;
}

void Bitmap::LoadReqListener(const Event* event)
{
	if (!reload_)
		return;
	//reload = false;
	//loadResource();
}
void Bitmap::UnloadReqListener(const Event* event)
{
	if (!IsLoaded())
		return;
	//reload = true;
	//unloadResource();
}

bool Bitmap::LoadResource()
{
	if (!IsLoaded())
	{
		bitmap_ = CreateID2D1BitmapFromDecoder(renderer_->GetRenderTarget(), decoder_);
		return IsLoaded();
	}
	return true;
}

bool Bitmap::UnloadResource()
{
	if (IsLoaded())
	{
		if (bitmap_) bitmap_->Release();
		bitmap_ = nullptr;
		return !IsLoaded();
	}
	return true;
}

bool Bitmap::IsLoaded()
{
	return bitmap_ != nullptr;
}