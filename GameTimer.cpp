#include "stdafx.h"
#include <windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
	: secondsPerCount(0.0), deltaTime(-1.0), baseTime(0),
	pausedTime(0), prevTime(0), currTime(0), stopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secondsPerCount = 1.0 / (double)countsPerSec;
}

float GameTimer::getTotalTime()
{
	if (stopped)
	{
		return (float)(((stopTime - pausedTime) - baseTime)*secondsPerCount);
	}
	else
	{
		return (float)(((currTime - pausedTime) - baseTime)*secondsPerCount);
	}
}

float GameTimer::getDeltaTime()
{
	return (float)deltaTime;
}

void GameTimer::reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	baseTime = currTime;
	prevTime = currTime;
	stopTime = 0;
	stopped = false;
}

void GameTimer::start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (stopped)
	{
		pausedTime += (startTime - stopTime);

		prevTime = startTime;
		stopTime = 0;
		stopped = false;
	}
}

void GameTimer::stop()
{
	if (!stopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		stopTime = currTime;
		stopped = true;
	}
}

void GameTimer::tick()
{
	if (stopped)
	{
		deltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	currTime = currTime;

	deltaTime = (currTime - prevTime)*secondsPerCount;

	prevTime = currTime;
	if (deltaTime < 0.0)
	{
		deltaTime = 0.0;
	}
}

