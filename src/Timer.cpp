#include "Timer.h"

Timer::Timer(float _duration) 
{
	this->startTime = GetTime();
	this->currentTime = GetTime();
	this->elapsedTime = 0;
	this->duration = _duration;
	this->endTime = currentTime + _duration;
	this->finished = false;
}

float Timer::GetStartTime() { return this->startTime; }
float Timer::GetCurrentTime() { return GetTime(); }
float Timer::GetElapsedTime() { return GetTime() - this->startTime; }
float Timer::GetEndTime() { return this->endTime; }
bool Timer::GetFinished() 
{
	CheckFinished();
	return this->finished; 
}
void Timer::SetFinished(bool newFinished) { this->finished = newFinished; }


bool Timer::CheckFinished() 
{

	if (this->currentTime >= this->endTime) 
	{
		return true;
	}
	else 
	{
		return false;
	}


}