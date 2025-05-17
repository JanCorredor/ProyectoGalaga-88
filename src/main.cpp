#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>
#include "Player.h"
#include "Particles.h"
#include"Enemy.h"
#include "Defines.h"
#include "HighScoreStorage.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Types and Structures Definitions
//-------------------------------------------------------------------------------------
typedef enum GameScreen 
{ 
	LOGO = 0, TITLE, STAGE1,STAGE2, BOSS, ENDING 
} GameScreen;

//-------------------------------------------------------------------------------------
// Global Variables 
//-------------------------------------------------------------------------------------
static Player player = Player::Player(); //Create Instance of Player

bool hardmode = false; //Is Hardmode on?

bool hasWon;
bool updatedScore = false;

bool isClean = false;

Timer enemyAttackTimer;
Timer logoTimer;
Timer timerSpawnEnemies;
Timer timerChangeStage;
Timer timerStageTitle;

//-------------------------------------------------------------------------------------
// Declaracion de funciones
//-------------------------------------------------------------------------------------

//Disparar
std::vector <Bullet> playerbullets; //Vector to control all the player bullets

int player_bullet_counter = 0; //How many bullets has the player shot
int hit_counter = 0; //How How many bullets hit an enemy
void DrawBullet();  //Update all the player bullets
GameScreen DevKeys(GameScreen current);

std::vector <Enemy> enemies;  //Vector to control all the enemies
Enemy callEnemyFunctions; // An auxiliary to call functions from the Enemy class
std::vector <Bullet> enemybullets;  //Vector to control all the enemy bullets


Boss boss;

void DrawEnemyBullet();//Update all the enemy bullets (Harmode off)
void DrawGodShot(); ////Update all the enemy bullets (Harmode on)
void EnemyManager(); //Manages enemy spawn waves

void ChangeStage(Timer timerChangeStage);

//-------------------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------------------
int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 1280, "Galaga'88"); // 1280, 800

    GameScreen currentScreen = LOGO;
    // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	//// Load textures from the resources directory
    //LOGO
    Texture GrupoDeNombreLogo = LoadTexture("Wiki/Sprites/GrupoDeNombre.png"); 


    //TITLE
    Texture Galaga88Logo = LoadTexture("HUD/GALAGA88_LOGO.png");

    //Gameplay
    ////HUD
    Texture stageindicator1 = LoadTexture("HUD/Galaga_'88_icon_stage_1.png");

    ////Enemies
    Texture Goei_0_t = LoadTexture("Enemies/Goei_0.png");
    Texture Goei_1_t = LoadTexture("Enemies/Goei_1.png");

    Texture Zako_t = LoadTexture("Enemies/Zako.png");
    Texture Bon_t = LoadTexture("Enemies/Bon.png");
    Texture Bos_t = LoadTexture("Enemies/Boos.png");

    Texture Boss_t = LoadTexture("Enemies/Galaga_'88_Giant Zako.png");

    ////Player
    Texture player_body_t = LoadTexture("Player/PlayerGalaga88.png");

    ////Bullets
    Texture player_bullet = LoadTexture("Player/PlayerBullet.png");
    Texture enemybullet_0 = LoadTexture("Enemies/BalaEnemigo_0.png");
    Texture enemybullet_1 = LoadTexture("Enemies/BalaEnemigo_1.png");
    Texture Bos_attack_t = LoadTexture("Enemies/BosAttack.png");

    //ResourceManager r;

    //Audio
    InitAudioDevice();      // Initialize audio device

    ////Ost and Sounds
    Sound buttonclick = LoadSound("Sounds/Click.wav");
    Sound GalagaOpening = LoadSound("Sounds/GalagaOpening.wav");
    Sound GalagaWin = LoadSound("Sounds/GalagaWin.wav");
    Sound GalagaDefeat = LoadSound("Sounds/GalagaDefeat.wav");

    ////Player
    Sound playerShoot = LoadSound("Sounds/PlayerShoot.wav");
    Sound playerDeath = LoadSound("Sounds/PlayerDeath.wav");

    ////Enemy
    Sound enemyShoot = LoadSound("Sounds/EnemyShoot.wav");
    Sound enemyFormation = LoadSound("Sounds/EnemyFormation.wav");
    Sound enemyDeathExplosion = LoadSound("Sounds/EnemyDeathExplosion.wav");


    SetTargetFPS(60); // Set Game to run 60 Frames per second

    //Particles
    CreateParticles();

    //-------------------------------------------------------------------------------------
    // Local Variables
    //-------------------------------------------------------------------------------------

    player.SetPosition({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() * 9 / 10 }); //Set the player position

    int spawnedWaves = 0;
    bool triggerTimerTitleStageOne = false;
    bool triggerTimerTitleStageTwo = false;

    //Creation of Timers (and startups)
    enemyAttackTimer.StartTimer(10.0);
    logoTimer.StartTimer(2.0);
    timerSpawnEnemies.StartTimer(5.0);

    //-------------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------------
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
        switch (currentScreen)
        {
        case LOGO:
        {
            PlaySound(GalagaOpening); 
            
            // Wait for 2 seconds (120 frames) before jumping to TITLE 
            if (logoTimer.CheckFinished() == true)
            {
                UnloadTexture(GrupoDeNombreLogo);
                currentScreen = TITLE;
            }
        } break;
        case TITLE:
        {
            currentScreen = DevKeys(currentScreen);
            if (IsKeyPressed(KEY_UP))
            {
                hardmode = true; //Hardmode on
            }
            if (IsKeyPressed(KEY_R))
            {
                ResetHighScore(); //Reset All Scores to 0
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                PlaySound(buttonclick);
                currentScreen = STAGE1; // STAGE 1 
                StopSound(GalagaOpening);
            }
        } break;
        case STAGE1:
        {
            //Enemies
            if (timerSpawnEnemies.CheckFinished() == true && spawnedWaves < 5)  //Spawn Enemy Waves
            {
                callEnemyFunctions.SpawnLevel1(&enemies, spawnedWaves);
                
                if (spawnedWaves == 0)
                {
                    enemies[0].original_position  = callEnemyFunctions.GetFormationPositions(7,5);//Zako
                    enemies[1].original_position  = callEnemyFunctions.GetFormationPositions(8,5);//Zako
                    enemies[2].original_position  = callEnemyFunctions.GetFormationPositions(8,6);//Zako
                    enemies[3].original_position  = callEnemyFunctions.GetFormationPositions(9,6);//Zako
                    enemies[4].original_position  = callEnemyFunctions.GetFormationPositions(1,5);//Zako
                    enemies[5].original_position  = callEnemyFunctions.GetFormationPositions(2,5);//Zako
                    enemies[6].original_position  = callEnemyFunctions.GetFormationPositions(0,6);//Zako
                    enemies[7].original_position  = callEnemyFunctions.GetFormationPositions(1,6);//Zako
                }
                else if (spawnedWaves == 1)
                {
                    enemies[8].original_position  = callEnemyFunctions.GetFormationPositions(3,3); // Goei
                    enemies[9].original_position  = callEnemyFunctions.GetFormationPositions(2,4); //Bon
                    enemies[10].original_position = callEnemyFunctions.GetFormationPositions(6,3);// Goei
                    enemies[11].original_position = callEnemyFunctions.GetFormationPositions(7,4);//Bon
                    enemies[12].original_position = callEnemyFunctions.GetFormationPositions(4,2);// Goei
                    enemies[13].original_position = callEnemyFunctions.GetFormationPositions(3,4);//Bon
                    enemies[14].original_position = callEnemyFunctions.GetFormationPositions(5,2);// Goei
                    enemies[15].original_position = callEnemyFunctions.GetFormationPositions(6,4);//Bon
                }
                else if (spawnedWaves == 2)
                {
                    enemies[16].original_position = callEnemyFunctions.GetFormationPositions(1,2); // Goei
                    enemies[17].original_position = callEnemyFunctions.GetFormationPositions(2,1); //Bos
                    enemies[18].original_position = callEnemyFunctions.GetFormationPositions(8,2);// Goei
                    enemies[19].original_position = callEnemyFunctions.GetFormationPositions(7,1);//Bos
                    enemies[20].original_position = callEnemyFunctions.GetFormationPositions(2,3);// Goei
                    enemies[21].original_position = callEnemyFunctions.GetFormationPositions(3,1);//Bos
                    enemies[22].original_position = callEnemyFunctions.GetFormationPositions(7,3);// Goei
                    enemies[23].original_position = callEnemyFunctions.GetFormationPositions(6,1);//Bos
                }
                else if (spawnedWaves == 3)
                {
                    enemies[24].original_position = callEnemyFunctions.GetFormationPositions(6,2); // Goei
                    enemies[25].original_position = callEnemyFunctions.GetFormationPositions(8,3); //Bon
                    enemies[26].original_position = callEnemyFunctions.GetFormationPositions(3,2);// Goei
                    enemies[27].original_position = callEnemyFunctions.GetFormationPositions(1,3);//Bon
                    enemies[28].original_position = callEnemyFunctions.GetFormationPositions(7,2);// Goei
                    enemies[29].original_position = callEnemyFunctions.GetFormationPositions(9,3);//Bon
                    enemies[30].original_position = callEnemyFunctions.GetFormationPositions(2,2);// Goei
                    enemies[31].original_position = callEnemyFunctions.GetFormationPositions(0,3);//Bon
                }
                else if (spawnedWaves == 4)
                {
                    enemies[32].original_position = callEnemyFunctions.GetFormationPositions(0,5);//Zako
                    enemies[33].original_position = callEnemyFunctions.GetFormationPositions(1,4);//Bon
                    enemies[34].original_position = callEnemyFunctions.GetFormationPositions(9,5);//Zako
                    enemies[35].original_position = callEnemyFunctions.GetFormationPositions(8,4);//Bon
                    enemies[36].original_position = callEnemyFunctions.GetFormationPositions(-1,5);//Zako
                    enemies[37].original_position = callEnemyFunctions.GetFormationPositions(4,4);//Bon
                    enemies[38].original_position = callEnemyFunctions.GetFormationPositions(10,5);//Zako
                    enemies[39].original_position = callEnemyFunctions.GetFormationPositions(5,4);//Bon
                }
                spawnedWaves++;
                timerSpawnEnemies.StartTimer(5.0);
            }
            EnemyManager();


            //Player
            currentScreen = DevKeys(currentScreen);
            player.Move();

            //Shoot
            if (IsKeyPressed(KEY_SPACE))
            {
                if (player.GetAlive() == true) {
                    PlaySound(playerShoot);
                    player_bullet_counter += 1;
                    player.Shoot(&playerbullets);
                }
            }

            //Enemy Collisions
            for (int i = 0; i < enemies.size(); i++)
            {
                for (int j = 0; j < playerbullets.size(); j++)
                {
                    if (enemies[i].IsEnemyAlive() == true) {
                        if (CheckCollisionCircles(playerbullets[j].bullet_position, playerbullets[j].bullet_radius, enemies[i].enemy_texture_position, enemies[i].GetEnemyRadius()))
                        {
                            player.SumScore(100);
                            hit_counter++;
                            PlaySound(enemyDeathExplosion);
                            enemies[i].SetEnemyLife(false);
                            playerbullets.erase(playerbullets.begin()+j);
                        }
                    }
                }
            }
            
            //Winning/Losing Conditions
            bool allDead = true;
            if (player.GetLives() < 0)
            {
                spawnedWaves = 0;
                hasWon = false;
                currentScreen = ENDING;
            }

            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies.empty() == false) 
                {
                    if (enemies[i].IsEnemyAlive() == true)
                    {
                        allDead = false;
                    }
                }
            }
            if (allDead == true && enemies.empty() == false && spawnedWaves == 5)
            {
                spawnedWaves = 0;
                enemies.clear();
                currentScreen = STAGE2;
            }

            //Score
            if (player.GetScore() > LoadHighScore(highestHighScore))
            {
                if (updatedScore == false)
                {
                    UpdateHighScore(player.GetScore());
                    updatedScore = true;
                }
                else
                {
                    SaveNewHighScore(highestHighScore, player.GetScore());
                }
            }

        } break;
        case STAGE2:
        {
            ChangeStage(timerChangeStage);

            if (timerChangeStage.CheckFinished() == true) 
            {
                   
            }

            //Enemies
            if (timerSpawnEnemies.CheckFinished() == true && spawnedWaves < 5)  //Spawn Enemy Waves
            {
                callEnemyFunctions.SpawnLevel2(&enemies, spawnedWaves);

                if (spawnedWaves == 0)
                {
                    enemies[0].original_position = callEnemyFunctions.GetFormationPositions(7, 5);//Zako
                    enemies[1].original_position = callEnemyFunctions.GetFormationPositions(8, 5);//Zako
                    enemies[2].original_position = callEnemyFunctions.GetFormationPositions(8, 6);//Zako
                    enemies[3].original_position = callEnemyFunctions.GetFormationPositions(9, 6);//Zako
                    enemies[4].original_position = callEnemyFunctions.GetFormationPositions(1, 5);//Zako
                    enemies[5].original_position = callEnemyFunctions.GetFormationPositions(2, 5);//Zako
                    enemies[6].original_position = callEnemyFunctions.GetFormationPositions(0, 6);//Zako
                    enemies[7].original_position = callEnemyFunctions.GetFormationPositions(1, 6);//Zako
                }
                else if (spawnedWaves == 1)
                {
                    enemies[8].original_position = callEnemyFunctions.GetFormationPositions(3, 3); // Goei
                    enemies[9].original_position = callEnemyFunctions.GetFormationPositions(6, 3);// Goei
                    enemies[10].original_position = callEnemyFunctions.GetFormationPositions(4, 2);// Goei
                    enemies[11].original_position = callEnemyFunctions.GetFormationPositions(5, 2);// Goei
                    enemies[12].original_position = callEnemyFunctions.GetFormationPositions(2, 4); //Bon
                    enemies[13].original_position = callEnemyFunctions.GetFormationPositions(7, 4);//Bon
                    enemies[14].original_position = callEnemyFunctions.GetFormationPositions(3, 4);//Bon
                    enemies[15].original_position = callEnemyFunctions.GetFormationPositions(6, 4);//Bon
                }
                else if (spawnedWaves == 2)
                {
                    enemies[16].original_position = callEnemyFunctions.GetFormationPositions(1, 2); // Goei
                    enemies[17].original_position = callEnemyFunctions.GetFormationPositions(8, 2);// Goei
                    enemies[18].original_position = callEnemyFunctions.GetFormationPositions(2, 3);// Goei
                    enemies[19].original_position = callEnemyFunctions.GetFormationPositions(7, 3);// Goei
                    enemies[20].original_position = callEnemyFunctions.GetFormationPositions(2, 1); //Bos
                    enemies[21].original_position = callEnemyFunctions.GetFormationPositions(7, 1);//Bos
                    enemies[22].original_position = callEnemyFunctions.GetFormationPositions(3, 1);//Bos
                    enemies[23].original_position = callEnemyFunctions.GetFormationPositions(6, 1);//Bos
                }
                else if (spawnedWaves == 3)
                {
                    enemies[24].original_position = callEnemyFunctions.GetFormationPositions(6, 2); // Goei
                    enemies[25].original_position = callEnemyFunctions.GetFormationPositions(3, 2);// Goei
                    enemies[26].original_position = callEnemyFunctions.GetFormationPositions(7, 2);// Goei
                    enemies[27].original_position = callEnemyFunctions.GetFormationPositions(2, 2);// Goei
                    enemies[28].original_position = callEnemyFunctions.GetFormationPositions(8, 3); //Bon
                    enemies[29].original_position = callEnemyFunctions.GetFormationPositions(1, 3);//Bon
                    enemies[30].original_position = callEnemyFunctions.GetFormationPositions(9, 3);//Bon
                    enemies[31].original_position = callEnemyFunctions.GetFormationPositions(0, 3);//Bon
                }
                else if (spawnedWaves == 4)
                {
                    enemies[32].original_position = callEnemyFunctions.GetFormationPositions(0, 5);//Zako
                    enemies[33].original_position = callEnemyFunctions.GetFormationPositions(9, 5);//Zako
                    enemies[34].original_position = callEnemyFunctions.GetFormationPositions(-1, 5);//Zako
                    enemies[35].original_position = callEnemyFunctions.GetFormationPositions(10, 5);//Zako
                    enemies[36].original_position = callEnemyFunctions.GetFormationPositions(1, 4);//Bon
                    enemies[37].original_position = callEnemyFunctions.GetFormationPositions(8, 4);//Bon
                    enemies[38].original_position = callEnemyFunctions.GetFormationPositions(4, 4);//Bon
                    enemies[39].original_position = callEnemyFunctions.GetFormationPositions(5, 4);//Bon
                }
                spawnedWaves++;
                timerSpawnEnemies.StartTimer(5.0);
            }

            EnemyManager();


            //Player
            currentScreen = DevKeys(currentScreen);
            player.Move();

            //Shoot
            if (IsKeyPressed(KEY_SPACE))
            {
                if (player.GetAlive() == true) {
                    PlaySound(playerShoot);
                    player_bullet_counter += 1;
                    player.Shoot(&playerbullets);
                }
            }

            //Enemy Collisions
            for (int i = 0; i < enemies.size(); i++)
            {
                for (int j = 0; j < playerbullets.size(); j++)
                {
                    if (enemies[i].IsEnemyAlive() == true) {
                        if (CheckCollisionCircles(playerbullets[j].bullet_position, playerbullets[j].bullet_radius, enemies[i].enemy_texture_position, enemies[i].GetEnemyRadius()))
                        {
                            player.SumScore(100);
                            hit_counter++;
                            PlaySound(enemyDeathExplosion);
                            enemies[i].SetEnemyLife(false);
                            playerbullets.erase(playerbullets.begin() + j);

                            //Score
                            if (player.GetScore() > LoadHighScore(highestHighScore))
                            {
                                if (updatedScore == false)
                                {
                                    UpdateHighScore(player.GetScore());
                                    updatedScore = true;
                                }
                                else
                                {
                                    SaveNewHighScore(highestHighScore, player.GetScore());
                                }
                            }
                        }
                    }
                }
            }

            //Winning/Losing Conditions
            bool allDead = true;
            if (player.GetLives() < 0)
            {
                spawnedWaves = 0;
                hasWon = false;
                currentScreen = ENDING;
            }

            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies.empty() == false)
                {
                    if (enemies[i].IsEnemyAlive() == true)
                    {
                        allDead = false;
                    }
                }
            }
            if (allDead == true && enemies.empty() == false && spawnedWaves == 5)
            {
                spawnedWaves = 0;
                hasWon = true;
                currentScreen = BOSS;
            }


        }break;
        case BOSS:
        {
            ChangeStage(timerChangeStage);

            if (timerChangeStage.CheckFinished() == true)
            {

            }

            //Enemy

            //Player
            currentScreen = DevKeys(currentScreen);
            player.Move();

            //Shoot
            if (IsKeyPressed(KEY_SPACE))
            {
                if (player.GetAlive() == true) {
                    PlaySound(playerShoot);
                    player_bullet_counter += 1;
                    player.Shoot(&playerbullets);
                }
            }

            //Enemy Collisions
            for (int i = 0; i < enemies.size(); i++)
            {
                for (int j = 0; j < playerbullets.size(); j++)
                {
                    if (boss.IsEnemyAlive() == true) {
                        if (CheckCollisionCircles(playerbullets[j].bullet_position, playerbullets[j].bullet_radius, boss.enemy_texture_position, boss.GetEnemyRadius()))
                        {
                            player.SumScore(100);
                            hit_counter++;
                            PlaySound(enemyDeathExplosion);
                            enemies[i].SetEnemyLife(false);
                            playerbullets.erase(playerbullets.begin() + j);

                            //Score
                            if (player.GetScore() > LoadHighScore(highestHighScore))
                            {
                                if (updatedScore == false)
                                {
                                    UpdateHighScore(player.GetScore());
                                    updatedScore = true;
                                }
                                else
                                {
                                    SaveNewHighScore(highestHighScore, player.GetScore());
                                }
                            }
                            if (boss.GetHP() <= 0) 
                            {
                                player.SumScore(1000);
                                hit_counter++;
                                PlaySound(enemyDeathExplosion);
                                playerbullets.erase(playerbullets.begin() + j);
                            }
                            else 
                            {
                                hit_counter++;
                                playerbullets.erase(playerbullets.begin() + j);
                                boss.GetHit();
                            }
                        }
                    }
                }
            }
            //Winning/Losing Conditions
            bool allDead = true;
            if (player.GetLives() < 0)
            {
                spawnedWaves = 0;
                hasWon = false;
                currentScreen = ENDING;
            }

            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies.empty() == false)
                {
                    if (enemies[i].IsEnemyAlive() == true)
                    {
                        allDead = false;
                    }
                }
            }
            if (allDead == true && enemies.empty() == false && spawnedWaves == 5)
            {
                spawnedWaves = 0;
                hasWon = true;
                currentScreen = ENDING;
            }


        }break;
        case ENDING:
        {
            currentScreen = DevKeys(currentScreen);
            if (isClean = false)
            {
                if (updatedScore == false)
                {
                    UpdateHighScore(player.GetScore());
                    updatedScore = true;
                }

                if (hasWon == true)
                {
                    PlaySound(GalagaWin);
                }
                else
                {
                    PlaySound(GalagaDefeat);
                }
                enemybullets.clear();
                playerbullets.clear();
                enemies.clear();
                isClean = true;
            }

            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                player = Player::Player();
                updatedScore = false;
                hardmode = false;
                StopSound(GalagaWin);
                StopSound(GalagaDefeat);
                player_bullet_counter = 0;
                hit_counter = 0;
                isClean = false;
                currentScreen = TITLE;
            }
        } break;
        default: break;
        }
        //-------------------------------------------------------------------------------------
        //DRAWING
        //-------------------------------------------------------------------------------------
		BeginDrawing();

        switch (currentScreen)
        {
        case LOGO:
        {
            ClearBackground(BLACK);
            DrawText("Jan Corredor", GetScreenWidth()/4, GetScreenHeight() * 5 / 10, 45, WHITE);
            DrawText("Arnau Gonzalez", GetScreenWidth()/4, GetScreenHeight() * 6 / 10, 45, WHITE);
            DrawText("Videogame Design and Development", GetScreenWidth()/100, GetScreenHeight() * 7 / 10, 45, WHITE);
            DrawText("UPC CITM TRS", GetScreenWidth() / 4, GetScreenHeight() * 8 / 10, 45, WHITE);
            DrawText("Tutor: Alejandro Paris Gomez", GetScreenWidth() / 10, GetScreenHeight() * 9 / 10, 45, WHITE);
            DrawTextureEx(GrupoDeNombreLogo, { (float)GetScreenWidth() / 10, 0 }, 0, 2, WHITE);
        } break;
        case TITLE:
        {
            ClearBackground(BLACK);

            //Particles
            DrawParticles();

            //Logo
            DrawTexture(Galaga88Logo, GetScreenWidth() / 4, GetScreenHeight()/ 10, WHITE);

            //Scores
            DrawText("SCORE", GetScreenWidth() / 20, GetScreenHeight() / 50, 45, WHITE);
            DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);

            DrawText("H I SCORE", GetScreenWidth() *7/ 10, GetScreenHeight() / 50, 45, WHITE); 
            DrawText(TextFormat("%i", LoadHighScore(highestHighScore)), GetScreenWidth() * 7 / 10, GetScreenHeight() / 20, 45, WHITE);

            //Other
            DrawText("PUSH ENTER", GetScreenWidth()/3, GetScreenHeight() /2, 45, GREEN);
        } break;
        case STAGE1:
        {
            ClearBackground(BLACK);

            //Particles
            DrawParticles();

            //Bullets
            DrawBullet();

            for (int i = 0; i < playerbullets.size(); i++) 
            {
                DrawTexture(player_bullet, playerbullets[i].bullet_position.x, playerbullets[i].bullet_position.y, WHITE);
            }

            if (hardmode == 0)
            {
                DrawEnemyBullet();
            }
            else
            {
                DrawGodShot();
            }

            for (int i = 0; i < enemybullets.size(); i++)
            {
                int x = GetRandomValue(0, 1);
                if (x == 0)
                {
                    DrawTexture(enemybullet_0, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
                }
                else
                {
                    DrawTexture(enemybullet_1, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
                }
            }

            //Player
            Vector2 playerActualPosition = player.GetPosition();
            if (player.GetAlive() == true) {
                if (player.GetInmortal())
                {
                    DrawTexture(player_body_t, playerActualPosition.x - 74, playerActualPosition.y - 63, BLUE);
                }
                else
                {
                    DrawTexture(player_body_t, playerActualPosition.x - 74, playerActualPosition.y - 63, WHITE);
                }
            }
            
            //Enemies
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].IsEnemyAlive() == true)
                {
                  
                    if (enemies[i].type == Goei) //Goei
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 33, enemies[i].GetEnemyPosition().y - 37 }; //-33 -37
                        DrawTextureEx(Goei_0_t, Correccion, enemies[i].texture_angle, 0.5, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Zako) //Zako
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32, enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Zako_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Bon) //Bon
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32 , enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Bon_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Bos) //Bos
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32, enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Bos_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                        if (enemies[i].aux == 1)
                        {
                            Vector2 Attack = { Correccion.x - 64 - 10, Correccion.y + 64 };
                            DrawTextureEx(Bos_attack_t, Attack, 0, 2, WHITE);
                        }
                    }
                    DrawCircle(enemies[i].enemy_texture_position.x, enemies[i].enemy_texture_position.y, enemies[i].GetEnemyRadius(), GREEN);
                }
                else
                {
                    if (enemies[i].type == Goei) //Goei
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 33, enemies[i].GetEnemyPosition().y - 37 }; //-33 -37
                        DrawTextureEx(Goei_0_t, Correccion, enemies[i].texture_angle, 0.5, enemies[i].DEAD);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Zako) //Zako
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32, enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Zako_t, Correccion, enemies[i].texture_angle, 1, enemies[i].DEAD);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Bon) //Bon
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32 , enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Bon_t, Correccion, enemies[i].texture_angle, 1, enemies[i].DEAD);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Bos) //Bos
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32, enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Bos_t, Correccion, enemies[i].texture_angle, 1, enemies[i].DEAD);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                }
            }

            //UI
            ////Scores
            DrawText("1UP", GetScreenWidth() / 13, GetScreenHeight() / 50, 45, YELLOW);
            DrawText("HIGH SCORE", GetScreenWidth() / 3, GetScreenHeight() / 50, 45, RED);

            DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
            DrawText(TextFormat("%i", LoadHighScore(highestHighScore)), GetScreenWidth() / 3, GetScreenHeight() / 20, 45, WHITE);

            //// Lives Remaining
            for (int i = 0; i < player.GetLives(); i++)
            {
                DrawTexture(player_body_t, 74 * i - GetScreenWidth() / 30, GetScreenHeight() * 9 / 10, WHITE);
            }

            ////Stage Indicator
            DrawTexture(stageindicator1, GetScreenWidth() * 95 / 100, GetScreenHeight() * 92 / 100, WHITE);

            //Starting level text
            if (triggerTimerTitleStageOne == false)
            {
                timerStageTitle.StartTimer(2.0);
                triggerTimerTitleStageOne = true;
            }
            if (timerStageTitle.CheckFinished() == false)
            {
                DrawText("STAGE 1", GetScreenWidth() * 9 / 24, GetScreenHeight() / 3, 45, GREEN);
            }


        } break;
        case STAGE2:
        {
            ClearBackground(BLACK);

            //Particulas
            DrawParticles();

            //EXPLOSION

            //UI
            ////Scores
            DrawText("1UP", GetScreenWidth() / 13, GetScreenHeight() / 50, 45, YELLOW);
            DrawText("HIGH SCORE", GetScreenWidth() / 3, GetScreenHeight() / 50, 45, RED);

            DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
            DrawText(TextFormat("%i", LoadHighScore(highestHighScore)), GetScreenWidth() / 3, GetScreenHeight() / 20, 45, WHITE);

            //// Lives Remaining
            for (int i = 0; i < player.GetLives(); i++)
            {
                DrawTexture(player_body_t, 74 * i - GetScreenWidth() / 30, GetScreenHeight() * 9 / 10, WHITE);
            }

            ////Stage Indicator
            DrawTexture(stageindicator1, GetScreenWidth() * 95 / 100, GetScreenHeight() * 92 / 100, WHITE);

            //Bullets
            DrawBullet();

            for (int i = 0; i < playerbullets.size(); i++) //Esto deberia estar en DrawBullets
            {
                DrawTexture(player_bullet, playerbullets[i].bullet_position.x, playerbullets[i].bullet_position.y, WHITE);
            }

            if (hardmode == 0)
            {
                DrawEnemyBullet();
            }
            else
            {
                DrawGodShot();
            }

            for (int i = 0; i < enemybullets.size(); i++) //Esto deberia estar en DrawBullets
            {
                int x = GetRandomValue(0, 1);
                if (x == 0)
                {
                    DrawTexture(enemybullet_0, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
                }
                else
                {
                    DrawTexture(enemybullet_1, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
                }
            }

            //Player
            Vector2 playerActualPosition = player.GetPosition();
            if (player.GetAlive() == true) {
                DrawTexture(player_body_t, playerActualPosition.x - 74, playerActualPosition.y - 63, WHITE);
            }

            //Enemies
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].IsEnemyAlive() == true)
                {
                    if (enemies[i].type == Goei) //Goei
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 33, enemies[i].GetEnemyPosition().y - 37 }; //-33 -37
                        DrawTextureEx(Goei_0_t, Correccion, enemies[i].texture_angle, 0.5, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Zako) //Zako
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32, enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Zako_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Bon) //Bon
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32 , enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Bon_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Bos) //Bos
                    {
                        Vector2 Correccion = { enemies[i].GetEnemyPosition().x - 32, enemies[i].GetEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Bos_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                }
            }
            if (triggerTimerTitleStageTwo == false)
            {
                timerStageTitle.StartTimer(2.0);
                triggerTimerTitleStageTwo = true;
            }
            if (timerStageTitle.CheckFinished() == false)
            {
                DrawText("STAGE 2", GetScreenWidth() * 9 / 24, GetScreenHeight() / 3, 45, GREEN);
            }


        } break;
        case BOSS:
        {
            ClearBackground(BLACK);

            //Particulas
            DrawParticles();

            //UI
            ////Scores
            DrawText("1UP", GetScreenWidth() / 13, GetScreenHeight() / 50, 45, YELLOW);
            DrawText("HIGH SCORE", GetScreenWidth() / 3, GetScreenHeight() / 50, 45, RED);

            DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
            DrawText(TextFormat("%i", LoadHighScore(highestHighScore)), GetScreenWidth() / 3, GetScreenHeight() / 20, 45, WHITE);

            //// Lives Remaining
            for (int i = 0; i < player.GetLives(); i++)
            {
                DrawTexture(player_body_t, 74 * i - GetScreenWidth() / 30, GetScreenHeight() * 9 / 10, WHITE);
            }

            ////Stage Indicator
            DrawTexture(stageindicator1, GetScreenWidth() * 95 / 100, GetScreenHeight() * 92 / 100, WHITE);

            //Bullets
            DrawBullet();

            for (int i = 0; i < playerbullets.size(); i++) //Esto deberia estar en DrawBullets
            {
                DrawTexture(player_bullet, playerbullets[i].bullet_position.x, playerbullets[i].bullet_position.y, WHITE);
            }

            if (hardmode == 0)
            {
                DrawEnemyBullet();
            }
            else
            {
                DrawGodShot();
            }

            for (int i = 0; i < enemybullets.size(); i++) //Esto deberia estar en DrawBullets
            {
                int x = GetRandomValue(0, 1);
                if (x == 0)
                {
                    DrawTexture(enemybullet_0, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
                }
                else
                {
                    DrawTexture(enemybullet_1, enemybullets[i].bullet_position.x, enemybullets[i].bullet_position.y, WHITE);
                }
            }

            //Player
            Vector2 playerActualPosition = player.GetPosition();
            if (player.GetAlive() == true) {
                DrawTexture(player_body_t, playerActualPosition.x - 74, playerActualPosition.y - 63, WHITE);
            }

            DrawCircle(boss.enemy_texture_position.x,boss.enemy_texture_position.y,boss.GetEnemyRadius(),GREEN);

            //Enemies
            boss.SetEnemyPosition({ 200,200 });
            Vector2 Correccion = { boss.GetEnemyPosition().x+128, boss.GetEnemyPosition().y+128}; //-32 -32

            Vector2 bossActualPosition = boss.GetEnemyPosition();
            boss.enemy_texture_position = Correccion;
            boss.CheckIsHit();
            if (boss.IsEnemyAlive() == true)
            {
                if (boss.GetIsHit() == true) 
                {
                    DrawTextureEx(Boss_t, bossActualPosition,0,1, RED);
                }
                else 
                {
                    DrawTextureEx(Boss_t, bossActualPosition, 0, 1, WHITE);
                }
            }
        }break;
        case ENDING:
        {
            ClearBackground(BLACK);
            DrawText("1UP", GetScreenWidth() / 13, GetScreenHeight() / 50, 45, YELLOW);
            DrawText("HIGH SCORE", GetScreenWidth() / 3, GetScreenHeight() / 50, 45, RED);

            DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
            DrawText(TextFormat("%i", (char*)LoadHighScore(highestHighScore)), GetScreenWidth() / 3, GetScreenHeight() / 20, 45, WHITE);

            DrawText("SHOTS FIRED", GetScreenWidth() / 13, GetScreenHeight() / 10, 45, YELLOW);
            DrawText(TextFormat("%i", (char*)player_bullet_counter), GetScreenWidth() * 2 / 3, GetScreenHeight() / 10, 45, YELLOW);

            DrawText("NUMBER OF HITS", GetScreenWidth() / 13, GetScreenHeight() / 7, 45, YELLOW);
            DrawText(TextFormat("%i", (char*)hit_counter), GetScreenWidth() * 2 / 3, GetScreenHeight() / 7, 45, YELLOW);

            float ratio;
            if (player_bullet_counter == 0)
            {
                ratio = 0.00f;
            }
            else
            {
                ratio = ((float)hit_counter / (float)player_bullet_counter) * 100;
            }

            DrawText("HIT-MISS RATIO", GetScreenWidth() / 13, GetScreenHeight() / 5, 45, YELLOW);
            DrawText(TextFormat("%d %%", (char)ratio), GetScreenWidth() * 2 / 3, GetScreenHeight() / 5, 45, YELLOW); ///////////////Error: No se dibujan los decimales

            DrawText("[1]", GetScreenWidth() / 13, GetScreenHeight() * 3 / 10, 50, GOLD);
            DrawText("[2]", GetScreenWidth() / 13, GetScreenHeight() * 4 / 10, 45, WHITE);
            DrawText("[3]", GetScreenWidth() / 13, GetScreenHeight() * 5 / 10, 45, WHITE);
            DrawText("[4]", GetScreenWidth() / 13, GetScreenHeight() * 6 / 10, 45, WHITE);
            DrawText("[5]", GetScreenWidth() / 13, GetScreenHeight() * 7 / 10, 45, WHITE);

            DrawText(TextFormat("%i", (char*)LoadHighScore(highestHighScore)), GetScreenWidth() / 6, GetScreenHeight() * 3 / 10, 50, GOLD);
            DrawText(TextFormat("%i", (char*)LoadHighScore(secondHighScore)), GetScreenWidth() / 6, GetScreenHeight() * 4 / 10, 40, WHITE);
            DrawText(TextFormat("%i", (char*)LoadHighScore(thirdHighScore)), GetScreenWidth() / 6, GetScreenHeight() * 5 / 10, 40, WHITE);
            DrawText(TextFormat("%i", (char*)LoadHighScore(fourthHighScore)), GetScreenWidth() / 6, GetScreenHeight() * 6 / 10, 40, WHITE);
            DrawText(TextFormat("%i", (char*)LoadHighScore(fifthHighScore)), GetScreenWidth() / 6, GetScreenHeight() * 7 / 10, 40, WHITE);

            if (hasWon == true)
            {
                DrawText("VICTORY ACHIEVED", GetScreenWidth() / 10, GetScreenHeight() * 16 / 20, 45, DARKGREEN);
                DrawText("Retry?", GetScreenWidth() / 10, GetScreenHeight() * 18 / 20, 45, GREEN);
            }
            else
            {
                DrawText("You DIED and LOST", GetScreenWidth() / 10, GetScreenHeight() * 16 / 20, 45, RED);
                DrawText("Retry?", GetScreenWidth() / 10, GetScreenHeight() * 18 / 20, 45, RED);
            }


        } break;
        default: break;
        }


		EndDrawing();
	}

	// Cleanup
	// Unload textures so it can be cleaned up TODO

    //Unload sounds
    UnloadSound(buttonclick);
    UnloadSound(playerShoot);

	//Destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
//-------------------------------------------------------------------------------------
// Functions 
//-------------------------------------------------------------------------------------

void DrawBullet()
{
    for (int i = 0; i < playerbullets.size(); i++) //Update
    {
        playerbullets[i].bullet_position.y -= 20; //Speed (20)
        if (playerbullets[i].bullet_position.y <= -20) //Sprite out of screen
        {
             playerbullets.erase(playerbullets.begin());
        }
    }  
}

void DrawEnemyBullet()
{
    for (int i = 0; i < enemybullets.size(); i++) //Update
    {
        enemybullets[i].bullet_position.y += 10; //Speed (10)

        if (ColorIsEqual(enemybullets[i].bullet_color, RED)) //Check bullet trajectory
        {
            enemybullets[i].bullet_position.x -= 0;
        }
        if (ColorIsEqual(enemybullets[i].bullet_color, ORANGE))
        {
            enemybullets[i].bullet_position.x -= 1;
        }
        if (ColorIsEqual(enemybullets[i].bullet_color, PURPLE))
        {
            enemybullets[i].bullet_position.x += 1;
        }

        if (enemybullets[i].bullet_position.y >= GetScreenHeight() + 20) //Sprite out of screen
        {
            enemybullets.erase(enemybullets.begin());
            continue;
        }

        if (CheckCollisionCircles(enemybullets[i].bullet_position, enemybullets[i].bullet_radius, player.GetPosition(), player.GetRadius())) //Check if bullet collides with player
        {
            enemybullets.erase(enemybullets.begin()+i);
            if (player.GetAlive() == true && player.GetInmortal() == false) 
            {
                player.Death(); // Player loses one life
            }
        }
    }
    player.CheckDeath(); //Check if player has lost all lives
}


void DrawGodShot()
{
    for (int i = 0; i < enemybullets.size(); i++) //Update
    {
        enemybullets[i].bullet_position.y += 10; //Speed (10)

        if (enemybullets[i].bullet_position.x == player.GetPosition().x) //Bullets change trajectory towards player
        {
            enemybullets[i].bullet_position.x -= 0;
        }
        else if (enemybullets[i].bullet_position.x > player.GetPosition().x)
        {
            enemybullets[i].bullet_position.x -= 3;
        }
        else
        {
            enemybullets[i].bullet_position.x += 3;
        }

        if (enemybullets[i].bullet_position.y >= GetScreenHeight() + 20) //Sprite out of screen
        {
            enemybullets.erase(enemybullets.begin());
            continue;
        }

        if (CheckCollisionCircles(enemybullets[i].bullet_position, enemybullets[i].bullet_radius, player.GetPosition(), player.GetRadius())) //Check if bullet collides with player
        {
            enemybullets.erase(enemybullets.begin() + i);
            if (player.GetAlive() == true && player.GetInmortal() == false)
            {
                player.Death(); // Player loses one life
            }
        }
    }
    player.CheckDeath(); //Check if player has lost all lives
}

void EnemyManager() 
{
    for (int count = 0; count < enemies.size(); count++) //Update all enemyes
    {
        if (enemies[count].IsEnemyAlive() == true)
        {
            if (enemies[count].inPosition[0] == false)
            {
                enemies[count].MoveToInAStraightLine(enemies[count].GetSemiCirclePoints(), 0); //Move from spawn to screen
            }
            else if (enemies[count].inPosition[1] == false)
            {
                enemies[count].SemiCircleMovement(); // Make the semicircle movement
            }
            else if (enemies[count].inPosition[2] == false)
            {
                enemies[count].MoveToInAStraightLine(enemies[count].original_position, 2); // Go to the assigned formation position
            }
            else if (enemies[count].inPosition[2] == true)
            {
                int rndmax = 0;
                for (int i = 0; i < enemies.size(); i++)
                {
                    if (enemies[i].inPosition[2] == true)
                    {
                        rndmax++;
                    }
                }
                int rnd = GetRandomValue(0, rndmax - 1);
                if (enemyAttackTimer.CheckFinished() && enemies[rnd].IsEnemyAlive() == true && enemies[rnd].inPosition[2] == true && enemies[rnd].inPosition[3] == false)
                {
                    enemies[rnd].Shoot(&enemybullets, player);         // Make a random enemy shoot
                    if (hardmode) { enemyAttackTimer.StartTimer(0.1); }
                    else { enemyAttackTimer.StartTimer(1.0); }
                    enemies[rnd].inPosition[3] = true;
                }
                if (enemies[count].inPosition[3] == true)
                {
                    enemies[count].Launch(&player); // Launch towards the player
                }
            }
        }
        else
        {
            enemies[count].inPosition[2] = true; //If enemy is dead check the positions so functions do not mess up

            if (enemies[count].DEAD.a != 0)
            {
                enemies[count].DEAD.a -= 1;
            }
            enemies[count].SetEnemyPosition(enemies[count].GetEnemyPosition().x, enemies[count].GetEnemyPosition().y + 1);
        }

    }

} 

void ChangeStage(Timer timerChangeStage) // Change Stage Animation
{
    timerChangeStage.StartTimer(5.0f);
    //Animation
    if (timerChangeStage.CheckFinished() == true) 
    {
        
    }
}

GameScreen DevKeys(GameScreen current)
{
    if (IsKeyPressed(KEY_I)) 
    {
        player.ToggleInmortal(true);
    }
    if (IsKeyPressed(KEY_O)) 
    {
        player.ToggleInmortal(false);
    }
    if (IsKeyPressed(KEY_ONE))
    {
        return STAGE1;
    }
    if (IsKeyPressed(KEY_TWO))
    {
        return STAGE2;
    }
    if (IsKeyPressed(KEY_THREE))
    {
        return BOSS;
    }
    if (IsKeyPressed(KEY_FOUR))
    {
        return ENDING;
        
    }
    if (IsKeyPressed(KEY_ZERO))
    {
        return TITLE;
    }
    return current;
}
