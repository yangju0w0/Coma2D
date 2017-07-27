/*
* filename	Sprite.h
* fileinfo	애니메이션 스프라이트 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include "Image.h"

COMA_NS_BEGIN

class Sprite :
	public Image
{
public:
	Sprite(Bitmap* bitmap, Size imageSize, Size tileSize, unsigned int widthNumber, unsigned int heightNumber, int tileNumber = 0, float frameChangeTime=0.01666666f, bool autoReplay=true);
	virtual ~Sprite();

	virtual void Render(ID2D1HwndRenderTarget* renderTarget, double deltaTime);
	virtual void Update(double deltaTime);

	void stop();
	void play();
	void gotoAndStop(unsigned int frame);
	void gotoAndPlay(unsigned int frame);
	void nextFrame();
	void prevFrame();

	void setAutoReplay(bool value){ autoReplay = value; }
	void setFrameChangeTime(float time){ frameChangeTime = time; }

	bool isPlaying(){ return playing; }
	unsigned int getCurrentFrame(){ return currentFrame; }
	unsigned int getTotalFrame(){ return totalFrame; }

private:
	Size tileSize;
	unsigned int currentFrame;
	unsigned int tempFrame;
	unsigned int totalFrame;
	unsigned int widthNumber;
	unsigned int heightNumber;
	bool autoReplay;
	float frameChangeTime;
	float timeCount;
	
	bool playing;
};

COMA_END