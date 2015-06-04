#pragma once

class GameTimer
{
public:
	GameTimer();

	float getTotalTime();
	float getDeltaTime();

	void reset();
	void start();
	void stop();
	void tick();

private:
	double secondsPerCount;
	double deltaTime;

	__int64 baseTime;
	__int64 pausedTime;
	__int64 stopTime;
	__int64 prevTime;
	__int64 currTime;

	bool stopped;
};