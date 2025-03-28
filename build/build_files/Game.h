#pragma once
#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>

typedef enum GameState
{
	LOGO = 0, TITLE, GAMEPLAY, ENDING
} GameState;

class Game 
{
private:
	GameState state;

public:
	Game();
	~Game();

	void Initialise();
	void Update();
	void Render();
	void Cleanup();

};