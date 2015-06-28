/*
* The MIT License (MIT)
*
* Copyright (c) 2015 Heonyang Ju
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*
* MIT 라이선스 (MIT)
*
* 저작권(C) 2015 주헌양
*
* 이 문서에 의해, 아래의 조건을 따르는 한 제한 없이 소프트웨어를 다루기 위해
* 소프트웨어의 복사본과 관련된 문서("소프트웨어")를 획득하는 사람에게 변경의
* 자유가 허용 된다. 제한 없음은 사용, 복제, 수정, 통합, 공포, 배포, 하위 라이선스
* 그리고/또는 소프트웨어의 복사본의 판매와 공급된 소프트웨어를 가진 사람에게
* 그렇게 할 수 있도록 허가하는 권한에 제한 없음을 포함 한다.
*
* 위의 저작권 표시와 이 허가 표시는 모든 소프트웨어의 복사본 또는 상당한 일부분에
* 포함되어야 한다.
*
* 이 소프트웨어는 특정한 목적과 침해 없음을 위해 상업성, 적합성의 보증에만
* 국한되는 것이 아닌 명시적 또는 묵시적인 어떠한 종류의 보증없이 "있는 그대로"
* 제공 된다. 어떠한 경우에도 저작자 또는 저작권자는 소프트웨어의 관계 안과 밖
* 또는 소프트웨어의 사용 또는 거래로부터 발생하는 계약, 불법 행위 또는 기타 수행에
* 대해, 어떠한 소송, 손해 또는 다른 책무에 대해서도 책임이 없다.
*
* 본문은 MIT라이선스 원문의 번역본이며, 모든 해석은 원문에 기초한다.
*/

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
		renderer->removeEventListener(RendererEvent::LOAD_RESOURCE_REQ, BIND(Bitmap::loadReqListener));
		renderer->removeEventListener(RendererEvent::UNLOAD_RESOURCES_REQ, BIND(Bitmap::unloadReqListener));
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

void Bitmap::loadReqListener(Event* event)
{
	if (!reload)
		return;
	reload = false;
	loadResource();
}
void Bitmap::unloadReqListener(Event* event)
{
	if (!isLoaded())
		return;
	reload = true;
	unloadResource();
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