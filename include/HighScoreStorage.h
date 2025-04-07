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

bool SaveStorageValue(unsigned int position, int value);
int LoadStorageValue(unsigned int position);