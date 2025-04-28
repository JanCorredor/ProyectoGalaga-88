#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>
#include "Player.h"
#include "Particles.h"
#include"Enemy.h"
#include "Defines.h"
#include "HighScoreStorage.h"

using namespace std;

//-------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Types and Structures Definitions
//-------------------------------------------------------------------------------------
typedef enum GameScreen 
{ 
	LOGO = 0, TITLE, GAMEPLAY, ENDING 
} GameScreen;

//-------------------------------------------------------------------------------------
// Global Variables 
//-------------------------------------------------------------------------------------
static Player player = Player::Player();

bool hardmode = false;

bool hasWon;
bool updatedScore = false;

//-------------------------------------------------------------------------------------
// Declaracion de funciones
//-------------------------------------------------------------------------------------

//Disparar
std::vector <Bullet> playerbullets;

int player_bullet_counter = 0;
int hit_counter = 0;
void DrawBullet();  //Update

std::vector <Enemy> enemies;
Enemy callEnemyFunctions;
std::vector <Bullet> enemybullets;

void DrawEnemyBullet();
void DrawGodShot(); //Harmode
void CheckGodMode();

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

    ////Player
    Texture player_body_t = LoadTexture("Player/PlayerGalaga88.png");

    ////Bullets
    Texture player_bullet = LoadTexture("Player/PlayerBullet.png");
    Texture enemybullet_0 = LoadTexture("Enemies/BalaEnemigo_0.png");
    Texture enemybullet_1 = LoadTexture("Enemies/BalaEnemigo_1.png");

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

    //Frame
    unsigned int framesCounter = 0;
    SetTargetFPS(60); // Set Game to run 60 Frames per second

    //Particles
    createParticles();

    //-------------------------------------------------------------------------------------
    // Local Variables
    //-------------------------------------------------------------------------------------
    //Player
    player.SetPosition({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() * 9 / 10 });

    Timer enemyAttackTimer;
    enemyAttackTimer.StartTimer(10.0);
    Timer logoTimer;
    logoTimer.StartTimer(2.0);

    Timer timerSpawnEnemies;
    timerSpawnEnemies.StartTimer(5.0);
    int oleadasSpawneadas = 0;
    //-------------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------------
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
        switch (currentScreen)
        {
        case LOGO:
        {
            PlaySound(GalagaOpening); 
            //framesCounter++;    // Count frames
            
            // Wait for 2 seconds (120 frames) before jumping to TITLE 
            if (logoTimer.CheckFinished() == true)
            {
                UnloadTexture(GrupoDeNombreLogo);
                currentScreen = TITLE;
            }
        } break;
        case TITLE:
        {
            if (IsKeyPressed(KEY_UP))
            {
                hardmode = true;
            }
            if (IsKeyPressed(KEY_ENTER)) //  || IsGestureDetected(GESTURE_TAP)
            {
                PlaySound(buttonclick);
                currentScreen = GAMEPLAY;
                StopSound(GalagaOpening); 
            }
            if (IsKeyPressed(KEY_R))
            {
                ResetHighScore();
            }
        } break;
        case GAMEPLAY:
        {
            if (timerSpawnEnemies.CheckFinished() == true && oleadasSpawneadas <= 5) 
            CheckGodMode();
            if (timerSpawnEnemies.CheckFinished() == true && oleadasSpawneadas < 5) 
            {
                callEnemyFunctions.spawnHorde(&enemies, 8, GetRandomValue(1, 6));
                oleadasSpawneadas++;
                timerSpawnEnemies.StartTimer(5.0);
            }

            for (int count = 0; count < enemies.size(); count++)
            {
                if (enemies[count].isEnemyAlive() == true)
                {
                    if (enemies[count].inPosition[0] == false)
                    {
                        enemies[count].moveToInAStraightLine(enemies[count].semiCirclePoints(),0);
                    }
                    else if (enemies[count].inPosition[1] == false)
                    {
                        enemies[count].semiCircleMovement();
                    }
                    else if (enemies[count].inPosition[2] == false)
                    {
                        int j = count / 10;
                        int i = count - (j * 10);
                        enemies[count].moveToInAStraightLine(enemies[count].formationPositions(i, j),2);
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
                        if (enemyAttackTimer.CheckFinished() && enemies[rnd].isEnemyAlive() == true && enemies[rnd].inPosition[2] == true)
                        {
                            enemies[rnd].shoot(&enemybullets, player);
                            if (hardmode) { enemyAttackTimer.StartTimer(0.1); }
                            else {
                            enemyAttackTimer.StartTimer(1.0);
                            }
                        }
                    }
                }
                else
                {
                    enemies[count].inPosition[2] = true;
                }

            }


            //Player
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
                    if (enemies[i].isEnemyAlive() == true) {
                        if (CheckCollisionCircles(playerbullets[j].bullet_position, playerbullets[j].bullet_radius, enemies[i].enemy_texture_position, enemies[i].getEnemyRadius()))
                        {
                            player.SumScore(100);
                            hit_counter++;
                            PlaySound(enemyDeathExplosion);
                            enemies[i].setEnemyLife(false);
                            playerbullets.erase(playerbullets.begin()+j);
                        }
                    }
                }
            }
            
            //Winning/Losing Conditions
            bool allDead = true;
            if (player.GetLives() < 0)
            {
                currentScreen = ENDING;
                hasWon = false; 
            }
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies.empty() == false) 
                {
                    if (enemies[i].isEnemyAlive() == true)
                    {
                        allDead = false;
                    }
                }
            }
            if (allDead == true && enemies.empty() == false && oleadasSpawneadas == 5)
            {
                oleadasSpawneadas = 0;
                hasWon = true;
                currentScreen = ENDING;
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
        case ENDING:
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

            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                player = Player::Player();
                updatedScore = false;
                hardmode = false;
                StopSound(GalagaWin);
                StopSound(GalagaDefeat);
                player_bullet_counter = 0;
                hit_counter = 0;
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

            //Particulas
            drawParticles();

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
        case GAMEPLAY:
        {
            ClearBackground(BLACK);

            //Particulas
            drawParticles();

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
                if (enemies[i].isEnemyAlive() == true)
                {
                    if (enemies[i].type == Goei) //Goei
                    {
                        Vector2 Correccion = { enemies[i].getEnemyPosition().x - 33, enemies[i].getEnemyPosition().y - 37 }; //-33 -37
                        DrawTextureEx(Goei_0_t, Correccion, enemies[i].texture_angle, 0.5, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Zako) //Zako
                    {
                        Vector2 Correccion = { enemies[i].getEnemyPosition().x - 32, enemies[i].getEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Zako_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                    else if (enemies[i].type == Bon) //Bon
                    {
                        Vector2 Correccion = { enemies[i].getEnemyPosition().x - 32 , enemies[i].getEnemyPosition().y - 32 }; //-32 -32
                        DrawTextureEx(Bon_t, Correccion, enemies[i].texture_angle, 1, WHITE);
                        enemies[i].enemy_texture_position = Correccion;
                    }
                }
            }

            } break;
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

	// cleanup
	// unload our texture so it can be cleaned up

    //Unload sounds
    UnloadSound(buttonclick);
    UnloadSound(playerShoot);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
//-------------------------------------------------------------------------------------
// Funciones 
//-------------------------------------------------------------------------------------

void DrawBullet()
{
    for (int i = 0; i < playerbullets.size(); i++) //Update
    {
        playerbullets[i].bullet_position.y -= 20; //Speed (20)
        if (playerbullets[i].bullet_position.y <= -20) //Sprite mas o menos fuera de pantalla
        {
             playerbullets.erase(playerbullets.begin());
        }
    }  
}

void createEnemies()
{
    for (int i = 0; i < 60; i++)
    {
        
        Enemy newEnemy;
        enemies.push_back(newEnemy);
        enemies[i].setEnemyPosition(enemies[i].startingPositions(i));
    }
}

void enemyshoot()
{
    Bullet newBullet;
    int i = GetRandomValue(0, enemies.size() -1);

    if (enemies[i].isEnemyAlive() == true)
    {
        newBullet.bullet_position = enemies[i].getEnemyPosition();

        if (newBullet.bullet_position.x == player.GetPosition().x)
        {
            newBullet.bullet_color = RED;
        }
        else if (newBullet.bullet_position.x > player.GetPosition().x)
        {
            newBullet.bullet_color = ORANGE; // MINUS
        }
        else
        {
            newBullet.bullet_color = PURPLE; // PLUS
        }

        newBullet.bullet_radius = 10;
        enemybullets.push_back(newBullet);
    }
}

void DrawEnemyBullet()
{
    for (int i = 0; i < enemybullets.size(); i++) //Update
    {
        enemybullets[i].bullet_position.y += 10; //Speed (10)

        if (ColorIsEqual(enemybullets[i].bullet_color, RED))
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

        if (enemybullets[i].bullet_position.y >= GetScreenHeight() + 20) //Sprite mas o menos fuera de pantalla
        {
            enemybullets.erase(enemybullets.begin());
            continue;
        }

        if (CheckCollisionCircles(enemybullets[i].bullet_position, enemybullets[i].bullet_radius, player.GetPosition(), player.GetRadius()))
        {
            enemybullets.erase(enemybullets.begin()+i);
            if (player.GetAlive() == true && player.GetInmortal() == false)
            {
                player.Death();
            }
        }
    }
    player.CheckDeath();
}

void DrawGodShot()
{
    for (int i = 0; i < enemybullets.size(); i++) //Update
    {
        enemybullets[i].bullet_position.y += 10; //Speed (10)

        if (enemybullets[i].bullet_position.x == player.GetPosition().x)
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

        if (enemybullets[i].bullet_position.y >= GetScreenHeight() + 20) //Sprite mas o menos fuera de pantalla
        {
            enemybullets.erase(enemybullets.begin());
            continue;
        }

        if (CheckCollisionCircles(enemybullets[i].bullet_position, enemybullets[i].bullet_radius, player.GetPosition(), player.GetRadius()))
        {
            enemybullets.erase(enemybullets.begin() + i);
            if (player.GetAlive() == true && player.GetInmortal() == false)
            {
                player.Death();
            }
        }
    }
    player.CheckDeath();
}

void CheckGodMode() 
{
    if (IsKeyPressed(KEY_I)) 
    {
        player.ToggleInmortal(true);
    }
    if (IsKeyPressed(KEY_O)) 
    {
        player.ToggleInmortal(false);
    }
}