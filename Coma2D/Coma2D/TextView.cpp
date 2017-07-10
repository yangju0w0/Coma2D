/*
* filename	TextView.cpp
* fileinfo	텍스트 출력 표시객체 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "TextView.h"

COMA_USING_NS

IDWriteFactory* TextView::factory = 0;
bool TextView::factoryAvailable = false;

TextView::TextView(Size layoutSize, std::wstring text, std::wstring fontName, float fontSize, Color color, int textAlign)
{
	if (!factoryAvailable)
		initFactory();
	
	factory->CreateTextFormat(fontName.c_str(), 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"ko", &format);
	setLocalSize(layoutSize);
	this->text = text;
	this->color = color;
	this->color.a = getScreenAlpha();
	this->textAlign = textAlign;
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
	if (brush) brush->Release();
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
		if (brush)brush->Release();
		brush = nullptr;
	}
	if (!brush)
	{
		renderTarget->CreateSolidColorBrush(color, &brush);
	}
	
	renderTarget->SetTransform(getScreenMatrix());
	Rect rect = { 0, 0, getLocalSize().width, getLocalSize().height };
	renderTarget->DrawTextW(text.c_str(), text.length(), format, rect, brush);
}

void TextView::setColor(Color color)
{
	this->color = color;
	color.a = getScreenAlpha();
	if (brush) brush->Release();
	brush = nullptr;
}
void TextView::setColor(float r, float g, float b)
{
	setColor(Color{ r, g, b });
}
void TextView::setText(std::wstring text)
{
	this->text = text;
}