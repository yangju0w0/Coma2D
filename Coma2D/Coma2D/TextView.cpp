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
	:format(nullptr), textBrush_(nullptr), text(text), color(color), tempScreenAlpha(0.0f), textAlign(textAlign)
{
	if (!factoryAvailable)
		initFactory();
	
	factory->CreateTextFormat(fontName.c_str(), 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"ko", &format);
	setLocalSize(layoutSize);
	this->color.a = getScreenAlpha();
	switch (textAlign)
	{
	case ALIGN_LEFT:
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case ALIGN_RIGHT:
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	case ALIGN_CENTER:
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	}
}


TextView::~TextView()
{
	if(factory) factory->Release();
	factoryAvailable = false;
	if (format)format->Release();
	if (textBrush_) textBrush_->Release();
}


void TextView::initFactory()
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

void TextView::render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::render(renderTarget, deltaTime);
	if (!isVisible())
		return;
	if (std::roundf(getScreenAlpha() * 1000) != tempScreenAlpha)
	{
		tempScreenAlpha = std::roundf(getScreenAlpha() * 1000);
		color.a = getScreenAlpha();
		if (textBrush_)textBrush_->Release();
		textBrush_ = nullptr;
	}
	if (!textBrush_)
	{
		renderTarget->CreateSolidColorBrush(color, &textBrush_);
	}
	
	renderTarget->SetTransform(getScreenMatrix());
	Rect rect = { 0, 0, getLocalSize().width, getLocalSize().height };
	renderTarget->DrawTextW(text.c_str(), text.length(), format, rect, textBrush_);
}

void TextView::setColor(Color color)
{
	this->color = color;
	color.a = getScreenAlpha();
	if (textBrush_) textBrush_->Release();
	textBrush_ = nullptr;
}
void TextView::setColor(float r, float g, float b)
{
	setColor(Color{ r, g, b });
}
void TextView::setText(const std::wstring& text)
{
	this->text = text;
}