#include "raylib.h"
#include <stdlib.h>  

#include "HighScoreStorage.h"

bool SaveNewHighScore(unsigned int position, int value)
{
    bool success = false;
    int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char* fileData = LoadFileData(highScore_DATA_FILE, &dataSize);
    unsigned char* newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position * sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1) * sizeof(int);
            newFileData = (unsigned char*)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int* dataPtr = (int*)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", highScore_DATA_FILE, dataSize, position * sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int* dataPtr = (int*)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(highScore_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        //TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", highScore_DATA_FILE, value);
    }
    else
    {
        //TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", highScore_DATA_FILE);

        dataSize = (position + 1) * sizeof(int);
        fileData = (unsigned char*)RL_MALLOC(dataSize);
        int* dataPtr = (int*)fileData;
        dataPtr[position] = value;

        success = SaveFileData(highScore_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        //TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", highScore_DATA_FILE, value);
    }

    return success;
}

int LoadHighScore(unsigned int position) // Load integer value from storage file (from defined position)
{
    int value = 0;
    int dataSize = 0;
    unsigned char* fileData = LoadFileData(highScore_DATA_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < ((int)(position * 4))) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", highScore_DATA_FILE, position);
        else
        {
            int* dataPtr = (int*)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        //TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", highScore_DATA_FILE, value);
    }

    return value;    //If requested position could not be found, value 0 is returned
}

void ResetHighScore()
{
    SaveNewHighScore(highestHighScore, 0);
    SaveNewHighScore(secondHighScore, 0);
    SaveNewHighScore(thirdHighScore, 0);
    SaveNewHighScore(fourthHighScore, 0);
    SaveNewHighScore(fifthHighScore, 0);
}

void UpdateHighScore(int playerScore)
{
    while (1)
    {
        if (playerScore >= LoadHighScore(highestHighScore))
        {
            SaveNewHighScore(fifthHighScore, LoadHighScore(fourthHighScore));
            SaveNewHighScore(fourthHighScore, LoadHighScore(thirdHighScore));
            SaveNewHighScore(thirdHighScore, LoadHighScore(secondHighScore));
            SaveNewHighScore(secondHighScore, LoadHighScore(highestHighScore));
            SaveNewHighScore(highestHighScore, playerScore);
            break;
        }
        else if (playerScore >= LoadHighScore(secondHighScore))
        {
            SaveNewHighScore(fifthHighScore, LoadHighScore(fourthHighScore));
            SaveNewHighScore(fourthHighScore, LoadHighScore(thirdHighScore));
            SaveNewHighScore(thirdHighScore, LoadHighScore(secondHighScore));
            SaveNewHighScore(secondHighScore, playerScore);
            break;
        }
        else if (playerScore >= LoadHighScore(thirdHighScore))
        {
            SaveNewHighScore(fifthHighScore, LoadHighScore(fourthHighScore));
            SaveNewHighScore(fourthHighScore, LoadHighScore(thirdHighScore));
            SaveNewHighScore(thirdHighScore, playerScore);
            break;
        }
        else if (playerScore >= LoadHighScore(fourthHighScore))
        {
            SaveNewHighScore(fifthHighScore, LoadHighScore(fourthHighScore));
            SaveNewHighScore(fourthHighScore, playerScore);
            break;
        }
        else if (playerScore >= LoadHighScore(fifthHighScore))
        {
            SaveNewHighScore(fifthHighScore, playerScore);
            break;
        }
    }
}