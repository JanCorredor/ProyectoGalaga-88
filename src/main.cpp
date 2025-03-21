#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include <vector>
#include "Player.h"
#include "Particles.h"

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

//Title + Gameplay
typedef struct Bullet {
    Vector2 bullet_position;
    int bullet_radius;
    Color bullet_color;
} Bullet;

typedef struct Enemy {
    Vector2 enemy_position;
    int enemy_radius;
    Color enemy_color;
    bool enemy_alive;
} Enemy;

//-------------------------------------------------------------------------------------
// Global Variables 
//-------------------------------------------------------------------------------------
static Player player = Player::Player();

bool hardmode = false;

//-------------------------------------------------------------------------------------
// Declaracion de funciones
//-------------------------------------------------------------------------------------

//Disparar
std::vector <Bullet> playerbullets;

int player_bullet_counter = 0;
void ShootBullet();  //Create
void DrawBullet();  //Update

#define MAXENEMIES 7
Enemy enemies[MAXENEMIES];
std::vector <Bullet> enemybullets;
void createEnemies();
void DrawEnemies();

void moveEnemiesCircle();

void enemyshoot();

void DrawEnemyBullet();
void DrawGodShot(); //Harmode


//-------------------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------------------
int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 1280, "Galaga'88"); // 1280, 800

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	////// Load textures from the resources directory
    //LOGO
    Texture GalagaTitleLogo = LoadTexture("HUD/GalagaTitleLogo.png");
    GameScreen currentScreen = LOGO;

    //TITLE
    Texture Galaga88Logo = LoadTexture("HUD/GALAGA88_LOGO.png");

    //Gameplay
    ////HUD
    Texture stageindicator1 = LoadTexture("HUD/Galaga_'88_icon_stage_1.png");


    ////Player
    Texture player_body = LoadTexture("Player/PlayerGalaga88.png");

    ////Enemies
    Texture enemy1_0 = LoadTexture("Enemies/Enemy1_0.png");
    Texture enemy1_1 = LoadTexture("Enemies/Enemy1_1.png");

    ////Bullets
    Texture player_bullet = LoadTexture("Player/PlayerBullet.png");
    Texture enemybullet_0 = LoadTexture("Enemies/BalaEnemigo_0.png");
    Texture enemybullet_1 = LoadTexture("Enemies/BalaEnemigo_1.png");

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


    //-------------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------------
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
        switch (currentScreen)
        {
        case LOGO:
        {
            framesCounter++;    // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            
            if (framesCounter > 120)
            {
                UnloadTexture(GalagaTitleLogo);
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
            }
        } break;
        case GAMEPLAY:
        {
            if (enemybullets.empty() == true)
            {
                createEnemies();
            }

            framesCounter++;

            //Player
            player.Move();

            //Shoot
            if (IsKeyPressed(KEY_SPACE))
            {
                PlaySound(playerShoot);
                ShootBullet(); //Crear Instancia de bala
            }


            moveEnemiesCircle();

            //Enemy Collisions
            for (int i = 0; i < MAXENEMIES; i++)
            {
                for (int j = 0; j < player_bullet_counter; j++)
                {
                    if (enemies[i].enemy_alive == true) {
                        if (CheckCollisionCircles(playerbullets[j].bullet_position, playerbullets[j].bullet_radius, enemies[i].enemy_position, enemies[i].enemy_radius))
                        {
                            player.SumScore(100);
                            enemies[i].enemy_alive = false;
                        }
                    }
                }
            }



        } break;
        case ENDING:
        {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
            {
                //currentScreen = TITLE;
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
            DrawText("Jan Corredor", GetScreenWidth()/3, GetScreenHeight() * 5 / 10, 45, WHITE);
            DrawText("Arnau Gonzalez", GetScreenWidth()/3, GetScreenHeight() * 6 / 10, 45, WHITE);
            DrawText("Alexandre Garcia", GetScreenWidth()/3, GetScreenHeight() * 7 / 10, 45, WHITE);
            DrawTexture(GalagaTitleLogo, 0, 0, WHITE);
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
                //Insertar Puntuacion
            DrawText("H I SCORE", GetScreenWidth() *7/ 10, GetScreenHeight() / 50, 45, WHITE);
                //Insertar Puntuacion Mas alta

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
            DrawText("HIGH SCORE", GetScreenWidth() /3, GetScreenHeight() / 50, 45, RED);

            DrawText("20000", GetScreenWidth() / 13, GetScreenHeight() / 20, 45, WHITE);
            DrawText(TextFormat("%i", (char*)player.GetScore()), GetScreenWidth() / 3, GetScreenHeight() / 20, 45, WHITE);

                //Insertar HIGHSCORE
             
            //// Lives Remaining
            for (int i = 0; i < player.GetLives(); i++)
            {
                DrawTexture(player_body, 74 * i - GetScreenWidth()/ 30, GetScreenHeight() * 9/10, WHITE);
            }


            ////Stage Indicator
            DrawTexture(stageindicator1, GetScreenWidth()*95/100, GetScreenHeight() * 92 / 100, WHITE);

            //Bullets
            DrawBullet();

            for (int i = 0; i < player_bullet_counter; i++) //Esto deberia estar en DrawBullets
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
            DrawTexture(player_body, playerActualPosition.x - 74, playerActualPosition.y - 63, WHITE);




            //Enemies
            DrawEnemies();

            for (int i = 0; i < MAXENEMIES; i++)
            {
                if (enemies[i].enemy_alive == true)
                {
                    DrawTexture(enemy1_0, enemies[i].enemy_position.x-70, enemies[i].enemy_position.y-74, WHITE);
                }
            }

        } break;
        case ENDING:
        {


        } break;
        default: break;
        }


		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(GalagaTitleLogo);

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



void ShootBullet()
{
    Bullet newBullet;
    Vector2 playerActualPosition = player.GetPosition();
    player_bullet_counter++;

    newBullet.bullet_position = { playerActualPosition.x - 7, playerActualPosition.y - 30};

    //newBullet.bullet_position = { player.GetPosition().x - 7, player.GetPosition().y - 30}; -----

    if (IsKeyDown(KEY_LEFT))
    {
        newBullet.bullet_position.x -= 9; //Player Speed
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        newBullet.bullet_position.x += 9; //Player Speed
    }

    newBullet.bullet_radius = 10;
    newBullet.bullet_color = BLUE;
    playerbullets.push_back(newBullet);
}

void DrawBullet()
{
    for (int i = 0; i < player_bullet_counter; i++) //Update
    {
        playerbullets[i].bullet_position.y -= 20; //Speed (20)
        if (playerbullets[i].bullet_position.y <= -20) //Sprite mas o menos fuera de pantalla
        {
             playerbullets.erase(playerbullets.begin());
            player_bullet_counter--;
        }
    }
  
    //El draw se hace en el main porque no detecta la textura aqui, corregir
}

void createEnemies()
{
    for (int i = 0; i < MAXENEMIES; i++)
    {
        enemies[i].enemy_position.y = 100;
        enemies[i].enemy_position.x = (GetScreenWidth() / MAXENEMIES) + (i * 100);
        enemies[i].enemy_alive = true;
        enemies[i].enemy_radius = 50;
    }
}


void DrawEnemies() 
{
    for (int i = 0; i < MAXENEMIES; i++)
    {   
        if (enemies[i].enemy_alive == true) 
        {
            DrawCircle(enemies[i].enemy_position.x, enemies[i].enemy_position.y, enemies[i].enemy_radius, RED);
        }
    }
}

float angle = 0;
double tiempoa = 0;
void moveEnemiesCircle()
{
    if (GetTime() - tiempoa > 0.05)
    {
        tiempoa = GetTime();
        for (int i = 0; i < MAXENEMIES; i++)
        {
            enemies[i].enemy_position.x += cos(angle) * 10; //Mover en circulos
            enemies[i].enemy_position.y += sin(angle) * 10; // radio del circulo = 10
            angle += 0.1;
        }
        enemyshoot();
    }
}

void enemyshoot()
{
    Bullet newBullet;
    int i = GetRandomValue(0, MAXENEMIES-1);

    if (enemies[i].enemy_alive == true)
    {
        newBullet.bullet_position = { enemies[i].enemy_position.x, enemies[i].enemy_position.y};

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

        if (CheckCollisionCircles(enemybullets[i].bullet_position, enemybullets[i].bullet_radius, player.GetPosition(), player.GetRadius()))
        {
            player.SetLives(player.GetLives() - 1);
        }

        if (enemybullets[i].bullet_position.y >= GetScreenHeight() + 20 ) //Sprite mas o menos fuera de pantalla
        {
            enemybullets.erase(enemybullets.begin());
        }
    }

    //El draw se hace en el main porque no detecta la textura aqui, corregir
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
        }
    }
}

