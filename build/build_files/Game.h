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
	bool hardmode;
	//---------------------
	Texture GalagaTitleLogo;
	Texture Galaga88Logo;
	Texture stageindicator1;
	Texture player_body;
	Texture enemy1_0;
	Texture enemy1_1;
	Texture player_bullet;
	Texture enemybullet_0;
	Texture enemybullet_1;

	////Ost and Sounds
	Sound buttonclick;
	Sound GalagaOpening;
	Sound GalagaWin;
	Sound GalagaDefeat;

	////Player
	Sound playerShoot;
	Sound playerDeath;

	////Enemy
	Sound enemyShoot;
	Sound enemyFormation;
	Sound enemyDeathExplosion;

	unsigned int framesCounter = 0;

	GameState currentScreen;
public:
	Game();
	~Game();

	void Initialise();
	void Update();
	void Render();
	void Cleanup();

};