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

	void Stop();
	void Play();
	void GotoAndStop(unsigned int frame);
	void GotoAndPlay(unsigned int frame);
	void NextFrame();
	void PrevFrame();

	void SetAutoReplay(bool value){ autoReplay_ = value; }
	void SetFrameChangeTime(float time){ frameChangeTime_ = time; }

	bool IsPlaying(){ return playing_; }
	unsigned int GetCurrentFrame(){ return currentFrame_; }
	unsigned int GetTotalFrame(){ return totalFrame_; }

private:
	Size tileSize_;
	unsigned int currentFrame_;
	unsigned int tempFrame_;
	unsigned int totalFrame_;
	unsigned int widthNumber_;
	unsigned int heightNumber_;
	bool autoReplay_;
	float frameChangeTime_;
	float timeCount_;
	
	bool playing_;
};

COMA_END