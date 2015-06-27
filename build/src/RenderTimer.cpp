#include "RenderTimer.h"


RenderTimer::RenderTimer()
	:baseTime(0), pausedTime(0), stopTime(0), prevTime(0), currentTime(0), secondsPerCount(0), deltaTime(0), running(false)
{
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	secondsPerCount = 1.0 / freq;
}


RenderTimer::~RenderTimer()
{
}

void RenderTimer::reset()
{
	running = false;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	baseTime = currentTime;
	stopTime = currentTime;
	prevTime = currentTime;
	pausedTime = 0;
}

void RenderTimer::start()
{
	if (running)
		return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	pausedTime += currentTime - stopTime;

	prevTime = currentTime;

	running = true;
}

void RenderTimer::stop()
{
	if (!running)
		return;
	
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	stopTime = currentTime;
	running = false;
}

void RenderTimer::tick()
{
	if (!running)
		return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	deltaTime = (currentTime - prevTime) * secondsPerCount;
	prevTime = currentTime;
}

double RenderTimer::getTotalTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	return (currentTime - baseTime)*secondsPerCount;
}

double RenderTimer::getRunningTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	if (!running)
		return (currentTime - baseTime - pausedTime - (currentTime-stopTime))*secondsPerCount;
	return (currentTime - baseTime - pausedTime)*secondsPerCount;
}

double RenderTimer::getPausedTime()
{
	if (!running)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		return (pausedTime + (currentTime - stopTime))*secondsPerCount;
	}
	return pausedTime*secondsPerCount;
}

double RenderTimer::getDeltaTime()
{
	return deltaTime;
}