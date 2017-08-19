/*
* filename	RenderTimer.h
* fileinfo	렌더링용 타이머 유틸리티 클래스 헤더 파일
* author	주헌양 (Heonyang Ju)
*/

#pragma once
#include <Windows.h>
class RenderTimer
{
public:
	RenderTimer();
	~RenderTimer();

public:
	void Reset();
	void Start();
	void Stop();
	void Tick();

	double GetDeltaTime();
	double GetTotalTime();
	double GetRunningTime();
	double GetPausedTime();

public:
	bool IsRunning(){ return running_; }

private:
	bool running_;
	
	double secondsPerCount_;
	double deltaTime_;

	__int64 baseTime_; // 시작 시간
	__int64 prevTime_; // 이전 시간
	__int64 currentTime_; // 현재 시간

	__int64 stopTime_; // 정지 시점의 시간
	__int64 pausedTime_; // 누적 정지 시간

};

