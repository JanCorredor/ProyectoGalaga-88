#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>

class Timer 
{
private:
    bool started;
    float startTime;
    float elapsedTime;
    float endTime;
    bool finished;
    float duration;
    
public:
    //Constructores
    Timer();

    float GetStartTime();
    float GetElapsedTime();
    float GetEndTime();
    bool GetFinished();
    void SetFinished(bool newFinished);
    //Funciones
    bool CheckFinished();
    void StartTimer(float _duration);
};