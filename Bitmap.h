#pragma once
#include "Resource.h"
#include "ComaDevice.h"
#include <wincodec.h>
namespace coma2d
{
	class Bitmap :
		public Resource
	{
	private:
		Bitmap(TCHAR* filename){ this->filename = filename; }
		Bitmap(IStream* stream){ this->stream = stream; }
		~Bitmap(){ unloadResource(); }
	public:
		static const std::string TYPE_BITMAP;
		static Bitmap* CreateBitmap(TCHAR* filename, bool load = true)
		{
			Bitmap* bitmap = new Bitmap(filename);
			if (load)
				bitmap->loadResource();
			ComaDevice::GetDevice()->getResourceManager()->registerResource(bitmap);
			return bitmap;
		}
		static Bitmap* CreateBitmap(IStream* stream, bool load = true)
		{
			Bitmap* bitmap = new Bitmap(stream);
			if (load)
				bitmap->loadResource();
			ComaDevice::GetDevice()->getResourceManager()->registerResource(bitmap);
			return bitmap;
		}

		bool loadResource();
		bool unloadResource();
		ID2D1Bitmap* getResource();
		std::string getResourceType(){ return TYPE_BITMAP; }
		bool isLoaded()
		{
			if (bitmap)
				return true;
			return false;
		}

	private:
		static ID2D1Bitmap* CreateBitmapFromStream(IStream* stream)
		{
			if (!ComaDevice::GetDevice()->isInitialized())
				return nullptr;
			if (!ComaDevice::GetDevice()->getRenderer()->getRenderTarget())
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
			hr = ComaDevice::GetDevice()->getRenderer()->getRenderTarget()->CreateBitmapFromWicBitmap(converter, 0, &bitmap);

			if (FAILED(hr) && bitmap) bitmap->Release();
			if (frameDecode)	frameDecode->Release();
			if (converter)		converter->Release();

			return bitmap;
		}
		static ID2D1Bitmap* CreateBitmapFromFile(TCHAR* filename)
		{
			if (!ComaDevice::GetDevice()->isInitialized())
				return nullptr;
			if (!ComaDevice::GetDevice()->getRenderer()->getRenderTarget())
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
			hr = ComaDevice::GetDevice()->getRenderer()->getRenderTarget()->CreateBitmapFromWicBitmap(converter, 0, &bitmap);

			if (FAILED(hr) && bitmap) bitmap->Release();
			if (frameDecode)	frameDecode->Release();
			if (converter)		converter->Release();

			return bitmap;
		}
		
		static IWICImagingFactory* factory;


		ID2D1Bitmap* bitmap;
		TCHAR* filename;
		IStream* stream;
	};
}