#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>

class Timer 
{
private:
    float startTime;
    float currentTime;
    float elapsedTime;
    float endTime;
    bool finished;
    float duration;
    
public:
    //Constructores
    Timer(float _endTime);

    float GetStartTime();
    float GetCurrentTime();
    float GetElapsedTime();
    float GetEndTime();
    bool GetFinished();
    void SetFinished(bool newFinished);
    //Funciones
    bool CheckFinished();
};