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
	Bitmap(ComaRenderer* renderer, IWICBitmapDecoder* decoder) :renderer(renderer), decoder(decoder)
	{
		loadResource();
		renderer->setEventListener(RendererEvent::LOAD_RESOURCE_REQ, BIND(Bitmap::loadReqListener));
		renderer->setEventListener(RendererEvent::UNLOAD_RESOURCES_REQ, BIND(Bitmap::unloadReqListener));
	}

//======================== Static Mathods ====================

public:
	virtual ~Bitmap();
	static Bitmap* createBitmap(ComaRenderer* renderer, TCHAR* filename);
	static Bitmap* createBitmap(ComaRenderer* renderer, IStream* stream);
private:
	static IWICBitmapDecoder* createBitmapDecoderFromFile(TCHAR* filename);
	static IWICBitmapDecoder* createBitmapDecoderFromStream(IStream* stream);
	static ID2D1Bitmap* createID2D1BitmapFromDecoder(ID2D1HwndRenderTarget* renderTarget, IWICBitmapDecoder* decoder);
	static IWICImagingFactory* factory;

//============================================================

public:
	ID2D1Bitmap* getResource(){ return bitmap; }
private:
	ComaRenderer* renderer;
	ID2D1Bitmap* bitmap;
	IWICBitmapDecoder* decoder;

	void loadResource();
	void unloadResource();
	bool isLoaded();
	
	void loadReqListener(Event* event);
	void unloadReqListener(Event* event);
};

COMA_END