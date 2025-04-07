#include "Timer.h"

Timer::Timer() 
{
	this->started = false;
}
float Timer::GetStartTime() { return this->startTime; }
float Timer::GetElapsedTime() { return GetTime() - this->startTime; }
float Timer::GetEndTime() { return this->endTime; }
void Timer::SetFinished(bool newFinished) { this->finished = newFinished; }


void Timer::StartTimer(float _duration) 
{
	this->started = true;
	this->startTime = GetTime();
	this->elapsedTime = 0;
	this->duration = _duration;
	this->endTime = GetTime() + _duration;
	this->finished = false;

}

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