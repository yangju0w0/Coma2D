#pragma once
#include <Windows.h>
class RenderTimer
{
public:
	RenderTimer();
	~RenderTimer();

public:
	void reset();
	void start();
	void stop();
	void tick();

	double getDeltaTime();
	double getTotalTime();
	double getRunningTime();
	double getPausedTime();

public:
	bool isRunning(){ return running; }

private:
	bool running;
	
	double secondsPerCount;
	double deltaTime;

	__int64 baseTime; // 시작 시간
	__int64 prevTime; // 이전 시간
	__int64 currentTime; // 현재 시간

	__int64 stopTime; // 정지 시점의 시간
	__int64 pausedTime; // 누적 정지 시간

};

