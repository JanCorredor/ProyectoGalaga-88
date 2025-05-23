#include "Timer.h"
//Auxiliar class to help control timings and seconds.
Timer::Timer() 
{
	this->started = false;
}
//Getters and setters
float Timer::GetStartTime() { return this->startTime; }
float Timer::GetElapsedTime() { return GetTime() - this->startTime; }
float Timer::GetEndTime() { return this->endTime; }
void Timer::SetFinished(bool newFinished) { this->finished = newFinished; }

//Starts the timer and variables
void Timer::StartTimer(float _duration) 
{
	this->started = true;
	this->startTime = GetTime();
	this->elapsedTime = 0;
	this->duration = _duration;
	this->endTime = GetTime() + _duration;
	this->finished = false;

}
//Checks if the timer has finished
bool Timer::CheckFinished() 
{
	if (this->started == true) 
	{
		if (GetTime() >= this->endTime)
		{
			this->finished = true;
			return true;
		}
		else
		{
			this->finished = false;
			return false;
		}
	}

}