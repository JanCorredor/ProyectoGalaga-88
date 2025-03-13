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
    bool visible; // 1 visible 0 invisible
    int particle_size;
    int particle_speed;
} Particle;

//Gameplay
typedef struct Player {
    Vector2 position;
    int radius;
    Color color;
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
#define MAXPARTICLES 100  //Particles in screen
Particle particles[MAXPARTICLES];
void createParticles(); //Create
void drawParticles();  //Update

//Disparar
std::vector <Bullet> playerbullets;

int player_bullet_counter = -1;
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
	InitWindow(1280, 800, "Galaga'88");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	////// Load textures from the resources directory
    //LOGO
    Texture GalagaTitleLogo = LoadTexture("HUD/GalagaTitleLogo.png");
    GameScreen currentScreen = LOGO;

    //TITLE
    Texture Galaga88Logo = LoadTexture("HUD/GALAGA88_LOGO.png");

    //Gameplay
    Texture player_body = LoadTexture("Player/PlayerGalaga88.png");
    Texture player_bullet = LoadTexture("Player/PlayerBullet.png");

    Texture enemy1_0 = LoadTexture("Enemies/Enemy1_0.png");
    Texture enemy1_1 = LoadTexture("Enemies/Enemy1_1.png");

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
    player.position = { (float)GetScreenWidth()/2, (float)GetScreenHeight()*9/10 };

    for (int i = 0; i < MAXENEMIES; i++)
    {
        enemies[i].enemy_position.y = 100;
        enemies[i].enemy_position.x = (GetScreenWidth()/MAXENEMIES)+(i * 100);
        enemies[i].enemy_alive = true;
        enemies[i].enemy_radius = 20;
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
                DrawCircle(player.position.x - 7, player.position.y - 30, 20, WHITE);
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
            DrawText("Jan Corredor, Alexandre Garcia, Arnau Gonzalez", 75, 750, 45, WHITE);
            DrawTexture(GalagaTitleLogo, 0, 0, WHITE);
        } break;
        case TITLE:
        {
            ClearBackground(BLACK);

            //Particulas
            drawParticles();

            //Logo
            DrawTexture(Galaga88Logo, GetScreenWidth() / 3, GetScreenHeight()/ 10, WHITE);

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

            //Scores
            DrawText("SCORE", GetScreenWidth() / 20, GetScreenHeight() / 50, 45, WHITE);
                //Insertar Puntuacion


            //Bullets
            DrawBullet();

            for (int i = 0; i < player_bullet_counter; i++) //Esto deberia estar en DrawBullets
            {
                DrawTexture(player_bullet, playerbullets[i].bullet_position.x, playerbullets[i].bullet_position.y, WHITE);
            }
            
            //Player
            DrawTexture(player_body, player.position.x - 74, player.position.y - 63, WHITE);


            DrawEnemies();

            for (int i = 0; i < MAXENEMIES; i++)
            {
                if (enemies[i].enemy_alive == true)
                {
                    DrawTexture(enemy1_0, enemies[i].enemy_position.x-70, enemies[i].enemy_position.y-70, WHITE);
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
        particles[i].particle_size = 5; // Constant Value
        particles[i].particle_speed = GetRandomValue(1, 3);
        particles[i].visible = true; //Visible by Default
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
    newBullet.bullet_radius = 10;
    newBullet.bullet_color = BLUE;
    playerbullets.push_back(newBullet);
}

void DrawBullet()
{
    for (int i = 0; i < player_bullet_counter; i++) //Update
    {
        playerbullets[i].bullet_position.y -= 20; //Speed (20)
    }
       
    //Falta eliminar balas despues de que salen de pantalla

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


int angle = 0;
void moveEnemiesCircle()
{
    double tiempoa = 0, tiempob = GetTime();
    bool circletime = false;

    if (tiempob - tiempoa > 3)
    {
        circletime = true;
        tiempoa = GetTime();
    }

    for (int i = 0; i < MAXENEMIES; i++)
    {
        if (circletime == true)
        {
            angle++;
            enemies[i].enemy_position.x += cos(angle) * 100; //Mover en circulos
            enemies[i].enemy_position.y += sin(angle) * 100; // radio del circulo = 10
            circletime = false;
        }
    }
}