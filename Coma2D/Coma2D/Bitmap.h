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
	Bitmap(ComaRenderer* renderer, IWICBitmapDecoder* decoder, bool load = false)
		:renderer_(renderer), decoder_(decoder), reload_(false)
	{
		if (load)
		{
			LoadResource();
		}

		renderer_->SetEventListener(RendererEvent::LOAD_RESOURCE_REQ, BIND(Bitmap::LoadReqListener), this);
		renderer_->SetEventListener(RendererEvent::UNLOAD_RESOURCES_REQ, BIND(Bitmap::UnloadReqListener), this);
	}

public:
	virtual ~Bitmap();
	static Bitmap* CreateBitmap(TCHAR* filename);
	static Bitmap* CreateBitmap(IStream* stream);
	static Bitmap* CreateBitmap(ComaRenderer* renderer, TCHAR* filename);
	static Bitmap* CreateBitmap(ComaRenderer* renderer, IStream* stream);

	static Bitmap* CreateBitmapAndLoad(TCHAR* filename);
	static Bitmap* CreateBitmapAndLoad(IStream* stream);
	static Bitmap* CreateBitmapAndLoad(ComaRenderer* renderer, TCHAR* filename);
	static Bitmap* CreateBitmapAndLoad(ComaRenderer* renderer, IStream* stream);

	static void SetRenderer(ComaRenderer* renderer);
private:
	static IWICBitmapDecoder* CreateBitmapDecoderFromFile(TCHAR* filename);
	static IWICBitmapDecoder* CreateBitmapDecoderFromStream(IStream* stream);
	static ID2D1Bitmap* CreateID2D1BitmapFromDecoder(ID2D1HwndRenderTarget* renderTarget, IWICBitmapDecoder* decoder);
	static IWICImagingFactory* factory;
	static ComaRenderer* mainRenderer;

public:
	ID2D1Bitmap* GetResource() const { return bitmap_; }
	bool IsLoaded();
	bool LoadResource();
	bool UnloadResource();

private:
	ComaRenderer* renderer_;
	ID2D1Bitmap* bitmap_;
	IWICBitmapDecoder* decoder_;
	bool reload_;

	void LoadReqListener(const Event* event);
	void UnloadReqListener(const Event* event);
};

COMA_END
