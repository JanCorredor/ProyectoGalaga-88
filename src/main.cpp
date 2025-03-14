/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <math.h>

#include <vector>


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
typedef struct {
    Vector2 particle_position;
    Color particle_color;
    bool visible = true; // visible, false = invisible
    int particle_size = 5;
    int particle_speed;
} Particle;

//Gameplay
typedef struct Player {
    Vector2 position;
    int radius;
    Color color;
    int lives = 2;
} Player;

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
static Player player = { 0 };

//-------------------------------------------------------------------------------------
// Declaracion de funciones
//-------------------------------------------------------------------------------------

//Particle System
#define MAXPARTICLES 200  //Particles in screen
Particle particles[MAXPARTICLES];
void createParticles(); //Create
void drawParticles();  //Update

//Disparar
std::vector <Bullet> playerbullets;

int player_bullet_counter = 0;
void ShootBullet();  //Create
void DrawBullet();  //Update

#define MAXENEMIES 5
Enemy enemies[MAXENEMIES];
void createEnemies();
void DrawEnemies();
void moveEnemiesCircle();


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

    //Frame
    unsigned int framesCounter = 0;
    SetTargetFPS(60); // Set Game to run 60 Frames per second

    //Particles
    createParticles();

    //-------------------------------------------------------------------------------------
    // Local Variables
    //-------------------------------------------------------------------------------------
    //Player
    player.radius = 25;
    player.position = { (float)GetScreenWidth()/2, (float)GetScreenHeight()*89/100 }; //9/10

    for (int i = 0; i < MAXENEMIES; i++)
    {
        enemies[i].enemy_position.y = 100;
        enemies[i].enemy_position.x = (GetScreenWidth()/MAXENEMIES)+(i * 100);
        enemies[i].enemy_alive = true;
        enemies[i].enemy_radius = 50;
    }


    //-------------------------------------------------------------------------------------
    // Game Loop
    //-------------------------------------------------------------------------------------
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
        switch (currentScreen)
        {
        case LOGO:
        {
            // TODO: Update LOGO screen variables here!

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
            // TODO: Update TITLE screen variables here!

            // Press enter to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER)) //  || IsGestureDetected(GESTURE_TAP)
            {
                currentScreen = GAMEPLAY;
            }
        } break;
        case GAMEPLAY:
        {
            framesCounter++;

            //Player Movement
            int player_speed = 9;
            if (IsKeyDown(KEY_LEFT))
            {
                player.position.x -= player_speed;
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                player.position.x += player_speed;
            }
            //Player Collisions
            if ((player.position.x + player.radius) >= GetScreenWidth()) //Right Side
            {
                player.position.x = GetScreenWidth() - player.radius;
            }
            else if ((player.position.x - player.radius) <= 0) //Left Side
            {
                player.position.x = 0 + player.radius;
            }

            //Shoot
            if (IsKeyPressed(KEY_SPACE))
            {
                ShootBullet(); //Crear Instancia de bala
            }

            moveEnemiesCircle();

            //Enemy Collisions
            for (int i = 0; i < MAXENEMIES; i++)
            {
                for (int j = 0; j < player_bullet_counter; j++)
                {
                    if (CheckCollisionCircles(playerbullets[j].bullet_position, playerbullets[j].bullet_radius,enemies[i].enemy_position,enemies[i].enemy_radius))
                    {
                        enemies[i].enemy_alive = false;
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

        //DRAWING
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
                //Insertar Puntuacion
            DrawText("HIGH SCORE", GetScreenWidth() /3, GetScreenHeight() / 50, 45, RED);
                //Insertar HIGHSCORE
             
            //// Lives Remaining
            for (int i = 0; i < player.lives; i++)
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
            
            //Player
            DrawTexture(player_body, player.position.x - 74, player.position.y - 63, WHITE);




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

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
//-------------------------------------------------------------------------------------
// Funciones 
//-------------------------------------------------------------------------------------

void createParticles()
{
    Color particle_colors[6] = { GREEN, SKYBLUE, BLUE, RED, WHITE, LIGHTGRAY }; //Particles Possible Colors

    for (int i = 0; i < MAXPARTICLES; i++) //Create
    {
        particles[i].particle_position = { (float)GetRandomValue(GetScreenWidth()/200, GetScreenWidth()*199/200), (float)GetRandomValue(GetScreenHeight() / 200, GetScreenHeight() * 199 / 200) }; //Random position in screen
        particles[i].particle_color = particle_colors[GetRandomValue(0, 5)]; // Pick Random Color
        particles[i].particle_speed = GetRandomValue(1, 3);
    }
}

void drawParticles()
{
    Color particle_colors[6] = { GREEN, SKYBLUE, BLUE, RED, WHITE, LIGHTGRAY }; //Particles Possible Colors

    for (int i = 0; i < MAXPARTICLES; i++) //Update
    {
        particles[i].particle_position.y += particles[i].particle_speed;

        if (GetRandomValue(0, 100) == 0) // Particles Blink
        {
            particles[i].visible = false; // Invisible
        }
        else if (GetRandomValue(0, 50) == 50)
        {
            particles[i].visible = true; // Visible
        }

        if ((particles[i].particle_position.y) >= GetScreenHeight()) // Restart 
        {
            particles[i].particle_position = {(float)GetRandomValue(GetScreenWidth() / 200, GetScreenWidth() * 199 / 200), (float)GetRandomValue(0, 5) };// Position
            particles[i].particle_color = particle_colors[GetRandomValue(0, 5)];                                                                     //Color
            particles[i].particle_speed = GetRandomValue(1, 3);                                                                                      //Speed
        }

    }

    for (int i = 0; i < MAXPARTICLES; i++) //Draw
    {
        if (particles[i].visible == true) //Draw only if visible
        {
            DrawRectangle(particles[i].particle_position.x, particles[i].particle_position.y, particles[i].particle_size, particles[i].particle_size, particles[i].particle_color);
        }
    }
}

void ShootBullet()
{
    Bullet newBullet;

    player_bullet_counter++;
    newBullet.bullet_position = { player.position.x - 7, player.position.y - 30 };

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
    }
}