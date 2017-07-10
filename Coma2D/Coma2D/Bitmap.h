/*
* filename	Bitmap.cpp
* fileinfo	비트맵 리소스 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Resource.h"
#include "ComaRenderer.h"
#include "RendererEvent.h"
#include <wincodec.h>
COMA_NS_BEGIN

class Bitmap :
	public Resource
{
private:
	Bitmap(ComaRenderer* renderer, IWICBitmapDecoder* decoder, bool load = false) :renderer(renderer), decoder(decoder), reload(false)
	{
		if (load)
			loadResource();
		renderer->setEventListener(RendererEvent::LOAD_RESOURCE_REQ,	BIND(Bitmap::loadReqListener), this);
		renderer->setEventListener(RendererEvent::UNLOAD_RESOURCES_REQ, BIND(Bitmap::unloadReqListener), this);
	}

//======================== Static Mathods ====================

public:
	virtual ~Bitmap();
	static Bitmap* createBitmap(TCHAR* filename);
	static Bitmap* createBitmap(IStream* stream);
	static Bitmap* createBitmap(ComaRenderer* renderer, TCHAR* filename);
	static Bitmap* createBitmap(ComaRenderer* renderer, IStream* stream);

	static Bitmap* createBitmapAndLoad(TCHAR* filename);
	static Bitmap* createBitmapAndLoad(IStream* stream);
	static Bitmap* createBitmapAndLoad(ComaRenderer* renderer, TCHAR* filename);
	static Bitmap* createBitmapAndLoad(ComaRenderer* renderer, IStream* stream);

	static void setRenderer(ComaRenderer* renderer);
private:
	static IWICBitmapDecoder* createBitmapDecoderFromFile(TCHAR* filename);
	static IWICBitmapDecoder* createBitmapDecoderFromStream(IStream* stream);
	static ID2D1Bitmap* createID2D1BitmapFromDecoder(ID2D1HwndRenderTarget* renderTarget, IWICBitmapDecoder* decoder);
	static IWICImagingFactory* factory;
	static ComaRenderer* mainRenderer;
//============================================================

public:
	ID2D1Bitmap* getResource(){ return bitmap; }
	bool isLoaded();
	bool loadResource();
	bool unloadResource();

private:
	ComaRenderer* renderer;
	ID2D1Bitmap* bitmap;
	IWICBitmapDecoder* decoder;
	bool reload;
	
	void loadReqListener(Event* event);
	void unloadReqListener(Event* event);
};

COMA_END