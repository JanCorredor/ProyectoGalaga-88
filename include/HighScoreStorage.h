#pragma once
#include "raylib.h"
#include <stdlib.h>  

#define highScore_DATA_FILE   "highScore.data"

typedef enum {
    highestHighScore = 0,
    secondHighScore = 1,
    thirdHighScore = 2,
    fourthHighScore = 3,
    fifthHighScore = 4
} StorageData;

bool SaveNewHighScore(unsigned int position, int value);
int LoadHighScore(unsigned int position);
void ResetHighScore();
void UpdateHighScore(int playerScore);