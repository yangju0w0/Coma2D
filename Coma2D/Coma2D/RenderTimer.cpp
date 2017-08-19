/*
* filename	RenderTimer.cpp
* fileinfo	렌더링용 타이머 유틸리티 클래스 구현 파일
* author	주헌양 (Heonyang Ju)
*/

#include "RenderTimer.h"


RenderTimer::RenderTimer()
	:baseTime_(0), pausedTime_(0), stopTime_(0), prevTime_(0), currentTime_(0), secondsPerCount_(0), deltaTime_(0), running_(false)
{
	__int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	secondsPerCount_ = 1.0 / freq;
}

RenderTimer::~RenderTimer()
{
}

void RenderTimer::Reset()
{
	running_ = false;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime_);
	baseTime_ = currentTime_;
	stopTime_ = currentTime_;
	prevTime_ = currentTime_;
	pausedTime_ = 0;
}

void RenderTimer::Start()
{
	if (running_)
		return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime_);
	pausedTime_ += currentTime_ - stopTime_;

	prevTime_ = currentTime_;

	running_ = true;
}

void RenderTimer::Stop()
{
	if (!running_)
		return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime_);
	stopTime_ = currentTime_;
	running_ = false;
}

void RenderTimer::Tick()
{
	if (!running_)
		return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime_);
	deltaTime_ = (currentTime_ - prevTime_) * secondsPerCount_;
	prevTime_ = currentTime_;
}

double RenderTimer::GetTotalTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime_);
	return (currentTime_ - baseTime_) * secondsPerCount_;
}

double RenderTimer::GetRunningTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime_);
	if (!running_)
	{
		return (currentTime_ - baseTime_ - pausedTime_ - (currentTime_ - stopTime_))*secondsPerCount_;
	}
	return (currentTime_ - baseTime_ - pausedTime_)*secondsPerCount_;
}

double RenderTimer::GetPausedTime()
{
	if (!running_)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime_);
		return (pausedTime_ + (currentTime_ - stopTime_)) * secondsPerCount_;
	}
	return pausedTime_ * secondsPerCount_;
}

double RenderTimer::GetDeltaTime()
{
	return deltaTime_;
}