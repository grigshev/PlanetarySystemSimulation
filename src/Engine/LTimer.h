#pragma once

#include <SDL.h>

//The application time based timer
class LTimer
{
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void tick();
	void stop();
	void pause();
	void unpause();
	void reset();

	//Gets the timer's time
	Uint32 getTicks();

	void calDeltaTime();
	double getDeltaTime();
	double getCurrentTime();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;

	//The timer status
	bool mPaused;
	bool mStarted;
};