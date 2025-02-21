/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

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
    int opacity;
    int particle_size;
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



//-------------------------------------------------------------------------------------
// Global Variables 
//-------------------------------------------------------------------------------------
static Player player = { 0 };

//-------------------------------------------------------------------------------------
// Declaracion de funciones
//-------------------------------------------------------------------------------------

#define MAXPARTICLES 50
Particle particles[MAXPARTICLES];
void createParticles();
void drawParticles();

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

	// Load textures from the resources directory
	Texture GalagaTitleLogo = LoadTexture("GalagaTitleLogo.png");
    Texture Galaga88Logo = LoadTexture("GALAGA88_LOGO.png");
    GameScreen currentScreen = LOGO;
    
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
    player.position = (Vector2){ GetScreenWidth()/2, GetScreenHeight()*9/10 };



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
            //Player Movement
            if (IsKeyDown(KEY_LEFT)) player.position.x -= 12;
            if (IsKeyDown(KEY_RIGHT)) player.position.x += 12;

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
                //Crear Instancia de bala
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
            DrawText("H I SCORE", GetScreenWidth() *7/ 10, GetScreenHeight() / 50, 45, WHITE);

            //Other
            DrawText("PUSH ENTER", GetScreenWidth()/3, GetScreenHeight() /2, 45, GREEN);





        } break;
        case GAMEPLAY:
        {
            ClearBackground(BLACK);

            DrawCircle(player.position.x, player.position.y, player.radius, BLUE);


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
        particles[i].particle_position = (Vector2){ GetRandomValue(GetScreenWidth()/100, GetScreenWidth()*99/100), 0};
        particles[i].particle_color = particle_colors[GetRandomValue(0, 5)];
        particles[i].particle_size = 5;
        particles[i].opacity = 100;
    }
}

void drawParticles()
{
    for (int i = 0; i < MAXPARTICLES; i++) //Update
    {
        particles[i].particle_position.y += 1;
    }

    for (int i = 0; i < MAXPARTICLES; i++) //Draw
    {
        DrawRectangle(particles[i].particle_position.x, particles[i].particle_position.y, particles[i].particle_size, particles[i].particle_size, particles[i].particle_color);
    }
}