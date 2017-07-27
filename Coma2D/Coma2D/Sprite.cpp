/*
* filename	Sprite.h
* fileinfo	애니메이션 스프라이트 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "Sprite.h"
#include "SpriteEvent.h"

COMA_USING_NS

Sprite::Sprite(Bitmap* bitmap, Size imageSize, Size tileSize, unsigned int widthNumber, unsigned int heightNumber, int tileNumber, float frameChangeTime, bool autoReplay)
	:Image(bitmap, imageSize), tileSize(tileSize), widthNumber(widthNumber), heightNumber(heightNumber), frameChangeTime(frameChangeTime), autoReplay(autoReplay), playing(true), currentFrame(1)
{
	if (tileNumber == 0)
		totalFrame = widthNumber*heightNumber;
	else
		totalFrame = tileNumber;
}


Sprite::~Sprite()
{}

void Sprite::Update(double deltaTime)
{
	if (isPlaying())
		timeCount += deltaTime;
	currentFrame = ((int)(timeCount / frameChangeTime) % totalFrame) + 1;
	if (currentFrame >= totalFrame && !autoReplay)
		stop();
	DisplayObject::Update(deltaTime);

	if (tempFrame != currentFrame)
	{
		tempFrame = currentFrame;
		DispatchEvent(new SpriteEvent(SpriteEvent::ENTER_FRAME, this));
	}
}
void Sprite::Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime)
{
	DisplayObject::Render(renderTarget, deltaTime);
	if (!IsVisible())
		return;
	if (bitmap->isLoaded())
	{
		renderTarget->SetTransform(GetScreenMatrix());
		D2D1_RECT_F size = { 0, 0, GetLocalSize().width, GetLocalSize().height };
		D2D1_RECT_F resSize = {
			((currentFrame - 1) % widthNumber)*tileSize.width,
			((currentFrame - 1) / widthNumber)*tileSize.height,
			((currentFrame - 1) % widthNumber)*tileSize.width + tileSize.width,
			((currentFrame - 1) / widthNumber)*tileSize.height + tileSize.height };
		renderTarget->DrawBitmap(bitmap->getResource(), size, GetScreenAlpha(), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, resSize);
	}
}

void Sprite::stop()
{
	if (!isPlaying())
		return;
	playing = false;
	timeCount = frameChangeTime * (currentFrame - 1) + frameChangeTime*0.05f;
	DispatchEvent(new SpriteEvent(SpriteEvent::STOPPED, this));
}
void Sprite::play()
{
	if (isPlaying())
		return;
	playing = true;
	timeCount = frameChangeTime * (currentFrame - 1) + frameChangeTime*0.05f;
	DispatchEvent(new SpriteEvent(SpriteEvent::PLAY, this));
}
void Sprite::gotoAndStop(unsigned int frame)
{
	currentFrame = frame;
	timeCount = frameChangeTime * (frame - 1) + frameChangeTime*0.05f;
	stop();
}
void Sprite::gotoAndPlay(unsigned int frame)
{
	currentFrame = frame;
	timeCount = frameChangeTime * (frame - 1) + frameChangeTime*0.05f;
	play();
}
void Sprite::nextFrame()
{
	if (currentFrame < totalFrame)
		currentFrame++;
}
void Sprite::prevFrame()
{
	if (currentFrame > 1)
		currentFrame--;
}