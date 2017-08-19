/*
* filename	Sprite.h
* fileinfo	애니메이션 스프라이트 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Sprite.h"
#include "SpriteEvent.h"

COMA_USING_NS

Sprite::Sprite(Bitmap* bitmap, Size imageSize, Size tileSize, unsigned int widthNumber, unsigned int heightNumber, int tileNumber, float frameChangeTime, bool autoReplay)
	:Image(bitmap, imageSize), tileSize_(tileSize), widthNumber_(widthNumber), heightNumber_(heightNumber), frameChangeTime_(frameChangeTime), autoReplay_(autoReplay), playing_(true), currentFrame_(1)
{
	if (tileNumber == 0)
	{
		totalFrame_ = widthNumber*heightNumber;
	}
	else
	{
		totalFrame_ = tileNumber;
	}
}


Sprite::~Sprite()
{}

void Sprite::Update(double deltaTime)
{
	if (IsPlaying())
		timeCount_ += deltaTime;
	currentFrame_ = ((int)(timeCount_ / frameChangeTime_) % totalFrame_) + 1;
	if (currentFrame_ >= totalFrame_ && !autoReplay_)
		Stop();
	DisplayObject::Update(deltaTime);

	if (tempFrame_ != currentFrame_)
	{
		tempFrame_ = currentFrame_;
		DispatchEvent(new SpriteEvent(SpriteEvent::ENTER_FRAME, this));
	}
}

void Sprite::Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::Render(renderTarget, deltaTime);
	if (!IsVisible())
		return;
	if (bitmap_->IsLoaded())
	{
		renderTarget->SetTransform(GetScreenMatrix());
		D2D1_RECT_F size = { 0, 0, GetLocalSize().width, GetLocalSize().height };
		D2D1_RECT_F resSize = {
			((currentFrame_ - 1) % widthNumber_)*tileSize_.width,
			((currentFrame_ - 1) / widthNumber_)*tileSize_.height,
			((currentFrame_ - 1) % widthNumber_)*tileSize_.width + tileSize_.width,
			((currentFrame_ - 1) / widthNumber_)*tileSize_.height + tileSize_.height };
		renderTarget->DrawBitmap(bitmap_->GetResource(), size, GetScreenAlpha(), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, resSize);
	}
}

void Sprite::Stop()
{
	if (!IsPlaying())
	{
		return;
	}
	playing_ = false;
	timeCount_ = frameChangeTime_ * (currentFrame_ - 1) + frameChangeTime_*0.05f;
	DispatchEvent(new SpriteEvent(SpriteEvent::STOPPED, this));
}

void Sprite::Play()
{
	if (IsPlaying())
		return;
	playing_ = true;
	timeCount_ = frameChangeTime_ * (currentFrame_ - 1) + frameChangeTime_*0.05f;
	DispatchEvent(new SpriteEvent(SpriteEvent::PLAY, this));
}

void Sprite::GotoAndStop(unsigned int frame)
{
	currentFrame_ = frame;
	timeCount_ = frameChangeTime_ * (frame - 1) + frameChangeTime_*0.05f;
	Stop();
}

void Sprite::GotoAndPlay(unsigned int frame)
{
	currentFrame_ = frame;
	timeCount_ = frameChangeTime_ * (frame - 1) + frameChangeTime_*0.05f;
	Play();
}

void Sprite::NextFrame()
{
	if (currentFrame_ < totalFrame_)
	{
		currentFrame_++;
	}
}

void Sprite::PrevFrame()
{
	if (currentFrame_ > 1)
	{
		currentFrame_--;
	}
}