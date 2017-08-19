/*
* filename	TextView.cpp
* fileinfo	텍스트 출력 표시객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "TextView.h"

COMA_USING_NS

IDWriteFactory* TextView::factory = 0;
bool TextView::factoryAvailable = false;

TextView::TextView(Size layoutSize, const std::wstring& text, const std::wstring& fontName, float fontSize, const Color& color, int textAlign)
	:format_(nullptr), textBrush_(nullptr), text_(text), color_(color), tempScreenAlpha_(0.0f), textAlign_(textAlign)
{
	if (!factoryAvailable)
	{
		InitFactory();
	}

	factory->CreateTextFormat(fontName.c_str(), 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"ko", &format_);
	SetLocalSize(layoutSize);
	this->color_.a = GetScreenAlpha();
	switch (textAlign)
	{
	case ALIGN_LEFT:
		format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case ALIGN_RIGHT:
		format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	case ALIGN_CENTER:
		format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	}
}


TextView::~TextView()
{
	if (factory) factory->Release();
	factoryAvailable = false;
	if (format_)format_->Release();
	if (textBrush_) textBrush_->Release();
}


void TextView::InitFactory()
{
	HRESULT hr;
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&factory
	);

	if (FAILED(hr))
		return;
	factoryAvailable = true;
}

void TextView::Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::Render(renderTarget, deltaTime);
	if (!IsVisible())
	{
		return;
	}

	if (std::roundf(GetScreenAlpha() * 1000) != tempScreenAlpha_)
	{
		tempScreenAlpha_ = std::roundf(GetScreenAlpha() * 1000);
		color_.a = GetScreenAlpha();
		if (textBrush_)textBrush_->Release();
		textBrush_ = nullptr;
	}

	if (!textBrush_)
	{
		renderTarget->CreateSolidColorBrush(color_, &textBrush_);
	}

	renderTarget->SetTransform(GetScreenMatrix());
	Rect rect = { 0, 0, GetLocalSize().width, GetLocalSize().height };
	renderTarget->DrawTextW(text_.c_str(), text_.length(), format_, rect, textBrush_);
}

void TextView::SetColor(Color color)
{
	this->color_ = color;
	color.a = GetScreenAlpha();
	if (textBrush_) textBrush_->Release();
	textBrush_ = nullptr;
}

void TextView::SetColor(float r, float g, float b)
{
	SetColor(Color{ r, g, b });
}

void TextView::SetText(const std::wstring& text)
{
	this->text_ = text;
}